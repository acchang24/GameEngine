#include "Renderer3D.h"
#include <iostream>
#include <glad/glad.h>
#include "../Animation/Skeleton.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "FrameBufferMultiSampled.h"
#include "Material.h"
#include "UniformBuffer.h"

// Window width
static int s_WindowWidth;
// Window height
static int s_WindowHeight;
// Bool for whenever the window is resized
static bool s_Resized = false;

Renderer3D::Renderer3D() :
	mMainFrameBuffer(nullptr),
	mBloomMaskFrameBuffer(nullptr),
	mBloomBlurHorizontalFrameBuffer(nullptr),
	mBloomBlurVerticalFrameBuffer(nullptr),
	mMaterialBuffer(nullptr),
	mSkeletonBuffer(nullptr),
	mWindow(nullptr),
	mContext(nullptr),
	mWindowTitle(),
	mNumSubsamples(0),
	mVSync(1),
	mIsFullScreen(false)
{
}

Renderer3D::~Renderer3D()
{
	std::cout << "Deleted Renderer3D\n";
}

Renderer3D* Renderer3D::Get()
{
	static Renderer3D s_Renderer3D;

	return &s_Renderer3D;
}

bool Renderer3D::Init(int width, int height, int subsamples, int vsync, bool fullscreen, SDL_bool mouseCaptured, const char* title)
{
	s_WindowWidth = width;
	s_WindowHeight = height;
	mNumSubsamples = subsamples;
	mVSync = vsync;
	mIsFullScreen = fullscreen;
	mWindowTitle = title;

	// Inititialize SDL for video and audio, and check if successful
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
		return false;
	}

	// Load default OpenGL library
	SDL_GL_LoadLibrary(NULL);

	// Set OpenGL attributes
	// Use core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Tell OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	// Specify OpenGL 4.5 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Request a color buffer with 8 bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Request a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Multisampling
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, mNumSubsamples);

	// Create the window
	if (mIsFullScreen)
	{
		mWindow = SDL_CreateWindow(
			mWindowTitle,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			0,
			0,
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
		);
	}
	else
	{
		mWindow = SDL_CreateWindow(
			mWindowTitle,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			s_WindowWidth,
			s_WindowHeight,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
	}

	// Check if window creation was successful
	if (!mWindow)
	{
		std::cout << "Failed to create a window " << SDL_GetError() << "\n";
		return false;
	}

	// Create OpenGL context using the new window
	mContext = SDL_GL_CreateContext(mWindow);

	if (mContext == NULL)
	{
		std::cout << "Failed to create an OpenGL context " << SDL_GetError() << "\n";
		return false;
	}

	// Obtain API function pointers for OpenGL/Initialize GLAD
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	std::cout << "OpenGL loaded\n";
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Graphics: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";

	// Enable v-sync by default
	SDL_GL_SetSwapInterval(mVSync);

	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(mouseCaptured);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// Callback function for when window is resized
	SDL_AddEventWatch(ResizeWindowEventWatcher, mWindow);

	// Enable z-buffering (depth testing)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable face culling
	glEnable(GL_CULL_FACE);

	// Enable anti-aliasing
	glEnable(GL_MULTISAMPLE);

	//// Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set viewport
	if (mIsFullScreen)
	{
		SDL_DisplayMode dm{};

		SDL_GetDesktopDisplayMode(0, &dm);

		s_WindowWidth = dm.w;
		s_WindowHeight = dm.h;
	}
	// Set the new viewport
	glViewport(0, 0, s_WindowWidth, s_WindowHeight);

	mMainFrameBuffer = CreateMultiSampledFrameBuffer(s_WindowWidth, s_WindowHeight, mNumSubsamples, "main_multisampled");
	mBloomMaskFrameBuffer = CreateFrameBuffer(s_WindowWidth, s_WindowHeight, "bloom_mask", 0.5f);
	mBloomBlurHorizontalFrameBuffer = CreateFrameBuffer(s_WindowWidth, s_WindowHeight, "bloom_blur_horizontal", 0.25f);
	mBloomBlurVerticalFrameBuffer = CreateFrameBuffer(s_WindowWidth, s_WindowHeight, "bloom_blur_vertical", 0.25f);

	mMaterialBuffer = new UniformBuffer(sizeof(MaterialColors), BufferBindingPoint::Material, "MaterialBuffer");
	mSkeletonBuffer = new UniformBuffer(sizeof(SkeletonConsts), BufferBindingPoint::Skeleton, "SkeletonBuffer");

	return true;
}

void Renderer3D::Shutdown()
{
	std::cout << "Shutting down Renderer3D\n";
	// Delete the OpenGL context
	SDL_GL_DeleteContext(mContext);
	// Destroy the window
	SDL_DestroyWindow(mWindow);
	// Quit SDL
	SDL_Quit();

	delete mMainFrameBuffer;
	delete mBloomMaskFrameBuffer;
	delete mBloomBlurHorizontalFrameBuffer;
	delete mBloomBlurVerticalFrameBuffer;

	delete mMaterialBuffer;
	delete mSkeletonBuffer;
}

void Renderer3D::ClearBuffers()
{
	if (s_Resized)
	{
		ResizeFrameBuffers();
	}

	// Specify color to clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Clear the color buffer, depth buffer for default frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer3D::SetFrameBuffer(FrameBuffer* framebuffer)
{
	//mMainFrameBuffer->SetActive();

	framebuffer->SetActive();

}

void Renderer3D::SetDefaultFrameBuffer() const
{
	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear the color buffer, depth buffer for this frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset viewport size to this frame buffer's dimensions
	glViewport(0, 0, s_WindowWidth, s_WindowHeight);
}

void Renderer3D::DrawFrameBuffers()
{
	mMainFrameBuffer->BlitBuffers();

	// Use the blitted texture and mask off dark spots
	mBloomMaskFrameBuffer->SetActive();
	mBloomMaskFrameBuffer->Draw(mMainFrameBuffer->GetTexture());
	// Use the masked off texture and blur horizontally
	mBloomBlurHorizontalFrameBuffer->SetActive();
	mBloomBlurHorizontalFrameBuffer->Draw(mBloomMaskFrameBuffer->GetTexture());
	// Use the horizontally blurred texture to blur vertically
	mBloomBlurVerticalFrameBuffer->SetActive();
	mBloomBlurVerticalFrameBuffer->Draw(mBloomBlurHorizontalFrameBuffer->GetTexture());
	// Uses the blitted texture and the blurred texture to additively blend them for final image
	SetDefaultFrameBuffer();
	mMainFrameBuffer->Draw(mMainFrameBuffer->GetTexture());
}

void Renderer3D::EndFrame()
{
	SDL_GL_SwapWindow(mWindow);
}

FrameBuffer* Renderer3D::GetFrameBuffer(const std::string& name)
{
	FrameBuffer* buffer = nullptr;

	if (mFrameBuffers.find(name) != mFrameBuffers.end())
	{
		buffer = mFrameBuffers[name];
	}

	return buffer;
}

FrameBuffer* Renderer3D::CreateFrameBuffer(int screenWidth, int screenHeight, const std::string& name, float size)
{
	FrameBuffer* framebuffer = new FrameBuffer(screenWidth, screenHeight, size);

	mFrameBuffers[name] = framebuffer;

	return framebuffer;
}

FrameBufferMultiSampled* Renderer3D::CreateMultiSampledFrameBuffer(int screenWidth, int screenHeight, int subsamples, const std::string& name, float size)
{
	FrameBufferMultiSampled* framebuffer = new FrameBufferMultiSampled(screenWidth, screenHeight, subsamples, size);

	mFrameBuffers[name] = framebuffer;

	return framebuffer;
}

void Renderer3D::SetFrameBufferShader(FrameBuffer* frameBuffer, Shader* shader)
{
	frameBuffer->SetShader(shader);
}

int Renderer3D::ResizeWindowEventWatcher(void* data, SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
		if (win == data)
		{
			SDL_GetWindowSize(win, &s_WindowWidth, &s_WindowHeight);
			printf("Window width: %i, Window height: %i\n", s_WindowWidth, s_WindowHeight);
			glViewport(0, 0, s_WindowWidth, s_WindowHeight);
			Camera::SetProjection(static_cast<float>(s_WindowWidth) / static_cast<float>(s_WindowHeight));

			s_Resized = true;
		}
	}
	return 0;
}

void Renderer3D::ResizeFrameBuffers()
{
	for (auto& fb : mFrameBuffers)
	{
		FrameBuffer* frameBuffer = fb.second;
		frameBuffer->Unload();

		FrameBufferMultiSampled* multiSampledFrameBuffer = dynamic_cast<FrameBufferMultiSampled*>(frameBuffer);
		if(multiSampledFrameBuffer)
		{
			multiSampledFrameBuffer->Load(s_WindowWidth, s_WindowHeight, mNumSubsamples);
		}
		else
		{
			fb.second->Load(s_WindowWidth, s_WindowHeight);
		}
	}

	s_Resized = false;
}

void Renderer3D::LinkShaderToUniformBlock(UniformBuffer* buffer, Shader* shader)
{
	buffer->LinkShader(shader);
}
