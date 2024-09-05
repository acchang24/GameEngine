#include "Game.h"
#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include "Entity/Entity3D.h"
#include "MemoryManager/AssetManager.h"
#include "Util/Random.h"

bool IS_FULLSCREEN = false;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
double MOUSE_SENSITIVITY = 0.05;
int SUB_SAMPLES = 4;
int VSYNC = 1;
const char* TITLE = "Game";

Game::Game() :
	mPrevKeyInputs(),
	mAssetManager(nullptr),
	mMousePosX(0),
	mMousePosY(0),
	mIsRunning(true),
	mMouseCaptured(SDL_FALSE) // TODO: Set this to SDL_TRUE if the game window captures the mouse
{
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	// TODO: Initialize the renderer here

	mAssetManager = AssetManager::Get();

	if (!LoadGameData())
	{
		return false;
	}

	return true;
}

void Game::Shutdown()
{
	// TODO: Call Shutdown() on the Renderer here

	for (auto e : mEntities)
	{
		delete e;
	}
	mEntities.clear();

	mAssetManager->Shutdown();
	mAssetManager = nullptr;
}

bool Game::LoadGameData()
{
	// TODO: LOAD GAME SPECIFIC ASSETS HERE

	return true;
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

	ProcessMouseInput();

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Game::ProcessMouseInput()
{
	// Calculate mouse movement
	int x = 0;
	int y = 0;
	SDL_GetRelativeMouseState(&x, &y);
	mMousePosX = x * MOUSE_SENSITIVITY;
	mMousePosY = -y * MOUSE_SENSITIVITY;
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
	
}
