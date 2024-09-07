#include "Game.h"
#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include "Entity/Entity3D.h"
#include "Graphics/Renderer2D.h"
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
	mRenderer(nullptr),
	mAssetManager(nullptr),
	mMousePosX(0),
	mMousePosY(0),
	mIsRunning(true),
	mMouseCaptured(SDL_FALSE)
{
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	mRenderer = Renderer2D::Get();
	mRenderer->Init(WINDOW_WIDTH, WINDOW_HEIGHT, IS_FULLSCREEN, mMouseCaptured, "Game");

	mAssetManager = AssetManager::Get();

	if (!LoadGameData())
	{
		return false;
	}

	return true;
}

void Game::Shutdown()
{
	mRenderer->Shutdown();

	UnloadGameData();

	mAssetManager->Shutdown();
	mAssetManager = nullptr;
}

bool Game::LoadGameData()
{
	// TODO: LOAD GAME SPECIFIC ASSETS HERE

	return true;
}

void Game::UnloadGameData()
{
	for (auto e : mEntities)
	{
		delete e;
	}
	mEntities.clear();
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
	Uint8 mouseButtonDown = 0;
	Uint8 mouseButtonUp = 0;
	Sint32 scrollDir = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			// Check to see when user quits the game
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			// Mouse click down
			mouseButtonDown = event.button.button;
			break;
		case SDL_MOUSEBUTTONUP:
			// Mouse click release
			mouseButtonUp = event.button.button;
			break;
		case SDL_MOUSEWHEEL:
			// Mouse wheel scroll
			scrollDir = event.wheel.y;
			break;
		}
	}

	ProcessMouseInput(mouseButtonDown, mouseButtonUp, scrollDir);

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

void Game::ProcessMouseInput(Uint8 buttonDown, Uint8 buttonUp, Sint32 scroll)
{
	// Single left click
	if (buttonDown == SDL_BUTTON_LEFT)
	{
		std::cout << "Left Click\n";
	}
	// Single right click
	if (buttonDown == SDL_BUTTON_RIGHT)
	{
		std::cout << "Right Click\n";
	}
	// Single scroll wheel click (mouse 3 click)
	if (buttonDown == SDL_BUTTON_MIDDLE)
	{
		std::cout << "Scroll Click\n";
	}
	// Single mouse 4 click
	if (buttonDown == SDL_BUTTON_X1)
	{
		std::cout << "Mouse 4 Click\n";
	}
	// Single mouse 5 click
	if (buttonDown == SDL_BUTTON_X2)
	{
		std::cout << "Mouse 5 Click\n";
	}

	Uint8 mouse_state = SDL_GetMouseState(NULL, NULL);
	// Left click hold
	if (mouse_state & SDL_BUTTON_LMASK)
	{
		std::cout << "Left Click hold\n";
	}
	// Right click hold
	if (mouse_state & SDL_BUTTON_RMASK)
	{
		std::cout << "Right Click hold\n";
	}
	// Scroll click hold
	if (mouse_state & SDL_BUTTON_MMASK)
	{
		std::cout << "Scroll Click hold\n";
	}
	// Mouse 4 click hold
	if (mouse_state & SDL_BUTTON_X1MASK)
	{
		std::cout << "Mouse 4 Click hold\n";
	}
	// Mouse 5 click hold
	if (mouse_state & SDL_BUTTON_X2MASK)
	{
		std::cout << "Mouse 5 Click hold\n";
	}

	// Left click release
	if (buttonUp == SDL_BUTTON_LEFT)
	{
		std::cout << "Left Click Release\n";
	}
	// Right click release
	if (buttonUp == SDL_BUTTON_RIGHT)
	{
		std::cout << "Right Click Release\n";
	}
	// Scroll click release
	if (buttonUp == SDL_BUTTON_MIDDLE)
	{
		std::cout << "Scroll Click Release\n";
	}
	// Mouse 4 click release
	if (buttonUp == SDL_BUTTON_X1)
	{
		std::cout << "Mouse 4 Click Release\n";
	}
	// Mouse 5 click release
	if (buttonUp == SDL_BUTTON_X2)
	{
		std::cout << "Mouse 5 Click Release\n";
	}

	// Scroll wheel up
	if (scroll >= 1)
	{
		std::cout << "Scroll Up\n";
	}
	// Scroll wheel down
	if (scroll <= -1)
	{
		std::cout << "Scroll Down\n";
	}

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
	mRenderer->ClearBuffers();

	mRenderer->Draw();

	mRenderer->EndFrame();
}

void Game::RemoveGameEntity(Entity* e)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), e);
	if (iter != mEntities.end())
	{
		// Swap to end of vector and pop off
		auto iter2 = mEntities.end() - 1;
		std::iter_swap(iter, iter2);
		delete e;
		mEntities.pop_back();
	}
}
