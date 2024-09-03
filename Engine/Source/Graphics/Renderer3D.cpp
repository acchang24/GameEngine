#include "Renderer3D.h"
#include <iostream>
#include <glad/glad.h>
#include "Camera.h"

// Window width
static int s_WindowWidth;
// Window height
static int s_WindowHeight;
// SDL_bool for if the mouse is captured within the renderer's window
static SDL_bool s_MouseCaptured;

Renderer3D::Renderer3D() :
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
	std::cout << "Delete Renderer3D\n";
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
	s_MouseCaptured = mouseCaptured;
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
	SDL_SetRelativeMouseMode(s_MouseCaptured);
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
	glViewport(0, 0, s_WindowWidth, s_WindowHeight);

	return true;
}

void Renderer3D::Shutdown()
{
	// Delete the OpenGL context
	SDL_GL_DeleteContext(mContext);
	// Destroy the window
	SDL_DestroyWindow(mWindow);
	// Quit SDL
	SDL_Quit();
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
		}
	}
	return 0;
}

void Renderer3D::ToggleMouseCapture()
{
	if (s_MouseCaptured == SDL_TRUE)
	{
		s_MouseCaptured = SDL_FALSE;
	}
	else
	{
		s_MouseCaptured = SDL_TRUE;
	}

	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(s_MouseCaptured);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);
}
