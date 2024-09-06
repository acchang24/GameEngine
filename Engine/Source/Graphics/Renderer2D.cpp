#include "Renderer2D.h"
#include <iostream>

// Window width
static int s_WindowWidth;
// Window height
static int s_WindowHeight;
// Bool for whenever the window is resized
static bool s_Resized = false;

Renderer2D::Renderer2D() :
	mWindow(nullptr),
	mRenderer(nullptr),
	mWindowTitle(),
	mIsFullScreen(false)
{
}

Renderer2D::~Renderer2D()
{
	std::cout << "Deleted Renderer2D\n";
}

Renderer2D* Renderer2D::Get()
{
	static Renderer2D s_Renderer2D;

	return &s_Renderer2D;
}

bool Renderer2D::Init(int width, int height, bool fullscreen, SDL_bool mouseCaptured, const char* title)
{
	s_WindowWidth = width;
	s_WindowHeight = height;
	mIsFullScreen = fullscreen;
	mWindowTitle = title;

	// Inititialize SDL for video and audio, and check if successful
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << "\n";
		return false;
	}

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


	// Create the renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	return true;
}

void Renderer2D::Shutdown()
{
	// Destroy the renderer
	SDL_DestroyRenderer(mRenderer);
	// Destroy the window
	SDL_DestroyWindow(mWindow);
	// Quit SDL
	SDL_Quit();
}
