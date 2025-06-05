#include "Renderer3D.h"
#include <iostream>
#include <glad/glad.h>
#include "../Animation/Skeleton.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "FrameBufferMultiSampled.h"
#include "Material.h"
#include "Shader.h"
#include "stb_image.h"
#include "VertexBuffer.h"
#include "../Animation/BoneData.h"

Renderer3D::Renderer3D() :
	mVertexBuffer(nullptr),
	mWindow(nullptr),
	mContext(nullptr),
	mWindowTitle(),
	mNumSubsamples(0),
	mVSync(1),
	mWindowWidth(0),
	mWindowHeight(0),
	mIsFullScreen(false)
{
}

Renderer3D::~Renderer3D()
{
	std::cout << "Deleted Renderer3D\n";
}

bool Renderer3D::Init(int width, int height, int subsamples, int vsync, bool fullscreen, SDL_bool mouseCaptured, const char* title)
{
	mWindowWidth = width;
	mWindowHeight = height;
	mNumSubsamples = subsamples;
	mVSync = vsync;
	mIsFullScreen = fullscreen;
	mWindowTitle = title;

	if (!InitSDL())
	{
		return false;
	}

	LoadOpenGL();

	if (!CreateWindow())
	{
		return false;
	}

	if (!CreateContext())
	{
		// Destroy window before returning false
		SDL_DestroyWindow(mWindow);
		return false;
	}

	LoadGLAD();

	// Flip loaded textures on the y axis by default
	stbi_set_flip_vertically_on_load(false);

	LoadSdlSettings(mouseCaptured);

	SetOpenGLCapabilities();

	// Vertex attributes for screen quad that fills the entire screen in Normalized Device Coordinates
	VertexScreenQuad quadVertices[] = 
	{
		glm::vec2(-1.0f,  1.0f), glm::vec2(0.0f, 1.0f),
		glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f),

		glm::vec2(-1.0f,  1.0f), glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f,  1.0f), glm::vec2(1.0f, 1.0f)
	};
	mVertexBuffer = new VertexBuffer(quadVertices, 0, sizeof(quadVertices), 0, sizeof(quadVertices) / sizeof(VertexScreenQuad), 0, VertexLayout::VertexScreenQuad);

	CreateUniformBuffer(sizeof(MaterialColors), BufferBindingPoint::Material, "MaterialBuffer");
	CreateUniformBuffer(sizeof(SkeletonConsts), BufferBindingPoint::Skeleton, "SkeletonBuffer");

	return true;
}

void Renderer3D::Shutdown()
{
	std::cout << "Shutting down Renderer3D\n";
	
	for (auto& ub : mUniformBuffers)
	{
		delete ub.second;
	}
	mUniformBuffers.clear();

	for (auto fb : mFrameBuffers)
	{
		delete fb;
	}
	mFrameBuffers.clear();

	delete mVertexBuffer;

	SDL_GL_DeleteContext(mContext);
	
	SDL_DestroyWindow(mWindow);
	
	SDL_Quit();
}

void Renderer3D::ClearBuffers()
{
	// Specify color to clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Clear the color buffer, depth buffer for default frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer3D::SetDefaultFrameBuffer() const
{
	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear the color buffer, depth buffer for this frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset viewport size to this frame buffer's dimensions
	glViewport(0, 0, mWindowWidth, mWindowHeight);
}

void Renderer3D::EndFrame()
{
	SDL_GL_SwapWindow(mWindow);
}

UniformBuffer* Renderer3D::CreateUniformBuffer(size_t bufferSize, BufferBindingPoint bindingPoint, const char* bufferName)
{
	UniformBuffer* buffer = new UniformBuffer(bufferSize, bindingPoint, bufferName);

	mUniformBuffers[bufferName] = buffer;

	return buffer;
}

UniformBuffer* Renderer3D::GetUniformBuffer(const std::string& bufferName)
{
	auto iter = mUniformBuffers.find(bufferName);

	if (iter != mUniformBuffers.end())
	{
		return iter->second;
	}

	return nullptr;
}

FrameBuffer* Renderer3D::CreateFrameBuffer(int width, int height, Shader* shader)
{
	FrameBuffer* framebuffer = new FrameBuffer(width, height, this, shader);

	mFrameBuffers.emplace_back(framebuffer);

	return framebuffer;
}

FrameBufferMultiSampled* Renderer3D::CreateMultiSampledFrameBuffer(int width, int height, int subsamples, Shader* shader)
{
	FrameBufferMultiSampled* framebuffer = new FrameBufferMultiSampled(width, height, subsamples, this, shader);

	mFrameBuffers.emplace_back(framebuffer);

	return framebuffer;
}

void Renderer3D::CreateBlend(Shader* shader, unsigned int texture1, unsigned int texture2, int textureUnit)
{
	shader->SetActive();

	// Bind the 1st texture
	shader->SetInt("screenTexture", textureUnit);
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Bind the 2nd texture
	int blurUnit = textureUnit + 3;
	shader->SetInt("blurTexture", blurUnit);
	glActiveTexture(GL_TEXTURE0 + blurUnit);
	glBindTexture(GL_TEXTURE_2D, texture2);
}

void Renderer3D::ResizeFrameBuffers()
{
	for (auto fb : mFrameBuffers)
	{
		fb->Unload();

		FrameBufferMultiSampled* multiSampledFrameBuffer = dynamic_cast<FrameBufferMultiSampled*>(fb);
		if(multiSampledFrameBuffer)
		{
			multiSampledFrameBuffer->Load(mWindowWidth * multiSampledFrameBuffer->GetWidthRatio(), mWindowHeight * multiSampledFrameBuffer->GetHeightRatio(), mNumSubsamples);
		}
		else
		{
			if (fb)
			{
				fb->Load(mWindowWidth * fb->GetWidthRatio(), mWindowHeight * fb->GetHeightRatio());
			}
		}
	}
}

bool Renderer3D::InitSDL() const
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

void Renderer3D::LoadOpenGL() const
{
	SDL_GL_LoadLibrary(NULL);

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
	// Enable multisampling
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, mNumSubsamples);
}

bool Renderer3D::CreateWindow()
{
	if (mIsFullScreen)
	{
		SDL_DisplayMode dm{};
		SDL_GetDesktopDisplayMode(0, &dm);
		mWindowWidth = dm.w;
		mWindowHeight = dm.h;

		mWindow = SDL_CreateWindow(
			mWindowTitle.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			mWindowWidth,
			mWindowHeight,
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
		);
	}
	else
	{
		mWindow = SDL_CreateWindow(
			mWindowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			mWindowWidth,
			mWindowHeight,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
		);
	}
	if (!mWindow)
	{
		std::cout << "Failed to create a window " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

bool Renderer3D::CreateContext()
{
	mContext = SDL_GL_CreateContext(mWindow);
	if (mContext == NULL)
	{
		std::cout << "Failed to create an OpenGL context " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

void Renderer3D::LoadGLAD() const
{
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	std::cout << "OpenGL loaded\n";
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Graphics: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
}

void Renderer3D::LoadSdlSettings(SDL_bool mouseCaptured) const
{
	// Enable v-sync by
	SDL_GL_SetSwapInterval(mVSync);
	// Capture the mouse
	SDL_SetRelativeMouseMode(mouseCaptured);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);
}

void Renderer3D::SetOpenGLCapabilities() const
{
	// Enable z-buffering (depth testing)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Enable face culling
	glEnable(GL_CULL_FACE);
	// Enable anti-aliasing
	glEnable(GL_MULTISAMPLE);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Set viewport using the window width and height
	glViewport(0, 0, mWindowWidth, mWindowHeight);
}
