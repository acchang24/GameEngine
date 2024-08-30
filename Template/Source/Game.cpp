#include "Game.h"
#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include "Entity/Entity3D.h"

const int IS_FULLSCREEN = 0;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
double MOUSE_SENSITIVITY = 0.05;
int SUBSAMPLES = 4;
int VSYNC = 1;
const char* TITLE = "Game";

Game::Game() :
	mPrevKeyInputs(),
	mWindow(nullptr),
	mContext(nullptr),
	mMousePosX(0.0),
	mMousePosY(0.0),
	mIsRunning(true)
{

}

Game::~Game()
{

}

bool Game::Init()
{
	if (!SetupOpenGL())
	{
		return false;
	}

	if (!LoadGameData())
	{
		return false;
	}

	return true;
}

bool Game::SetupOpenGL()
{
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
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, SUBSAMPLES);

	// Create the window
	if (IS_FULLSCREEN)
	{
		mWindow = SDL_CreateWindow(
			TITLE,
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
			TITLE,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
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
	SDL_GL_SetSwapInterval(VSYNC);

	// Enable z-buffering (depth testing)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable face culling
	glEnable(GL_CULL_FACE);

	// Enable anti-aliasing
	glEnable(GL_MULTISAMPLE);

	return true;
}

bool Game::LoadGameData()
{
	// LOAD GAME SPECIFIC ASSETS HERE

	return true;
}

void Game::Shutdown()
{
	for (auto e : mEntities)
	{
		delete e;
	}
	mEntities.clear();

	// Delete the OpenGL context
	SDL_GL_DeleteContext(mContext);
	// Destroy the window
	SDL_DestroyWindow(mWindow);
	// Quit SDL
	SDL_Quit();
}

void Game::Run()
{
	std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();

	while (mIsRunning)
	{
		// Calculate delta time
		std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
		double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count());
		float deltaTime = static_cast<float>(0.000000001 * duration);
		frameStart = frameEnd;

		ProcessInput();

		Update(deltaTime);

		Render();
	}
}

void Game::ProcessInput()
{
	// Check to see when user quits the game
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Calculate mouse movement
	int x = 0;
	int y = 0;
	SDL_GetRelativeMouseState(&x, &y);
	mMousePosX = x * MOUSE_SENSITIVITY;
	mMousePosY = y * MOUSE_SENSITIVITY;

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Game::Update(float deltaTime)
{
	for (auto e : mEntities)
	{
		e->Update(deltaTime);
	}
}

void Game::Render()
{
	// Specify color to clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// Clear the color buffer, depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto e : mEntities)
	{
		e->Draw();
	}

	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}
