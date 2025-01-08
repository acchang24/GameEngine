#include "Game.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include "Entity/Entity3D.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer3D.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexLayouts.h"
#include "MemoryManager/AssetManager.h"
#include "Util/Random.h"
#include "Profiler/Profiler.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
int SUB_SAMPLES = 4;
int VSYNC = 1;
double MOUSE_SENSITIVITY = 0.05;
bool IS_FULLSCREEN = false;
const char* TITLE = "Renderer01";

Game::Game() :
	mPrevKeyInputs(),
	mCamera(nullptr),
	mVertexBuffer(nullptr),
	mShader(nullptr),
	mWallTexture(nullptr),
	mRenderer(nullptr),
	mAssetManager(nullptr),
	mModel(glm::mat4(1.0f)),
	mTimer(0.0f),
	mMousePosX(0),
	mMousePosY(0),
	mIsRunning(true),
	mMouseCaptured(SDL_TRUE)
{
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	mRenderer = Renderer3D::Get();
	mRenderer->Init(WINDOW_WIDTH, WINDOW_HEIGHT, SUB_SAMPLES, VSYNC, IS_FULLSCREEN, mMouseCaptured, TITLE);

	mAssetManager = AssetManager::Get();

	mCamera = new Camera();
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	if (!LoadGameData())
	{
		return false;
	}

	return true;
}

void Game::Shutdown()
{
	delete mCamera;

	delete mVertexBuffer;

	mRenderer->Shutdown();

	UnloadGameData();

	mAssetManager->Shutdown();
	mAssetManager = nullptr;
}

bool Game::LoadGameData()
{
	VertexTexture vertices[] = {
		glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f),
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f),
		glm::vec3(-0.5f,  -0.5f, 0.0f), glm::vec2(0.0f, 0.0f),
		glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(0.0f, 1.0f),
	};
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	mVertexBuffer = new VertexBuffer(vertices, indices, sizeof(vertices), sizeof(indices), sizeof(vertices) / sizeof(VertexTexture), sizeof(indices) / sizeof(unsigned int), VertexLayout::VertexTexture);

	mShader = new Shader("Shaders/texture.vert", "Shaders/texture.frag");
	mAssetManager->SaveShader("color", mShader);

	mWallTexture = AssetManager::LoadTexture("Assets/Textures/wood.png");
	mWallTexture->SetType(TextureType::Diffuse);

	mAssetManager->ClearShaderPrograms();

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
		Profiler::Get()->ResetAll();

		PROFILE_SCOPE(GAME_LOOP);

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
	}
	// Single right click
	if (buttonDown == SDL_BUTTON_RIGHT)
	{
	}
	// Single scroll wheel click (mouse 3 click)
	if (buttonDown == SDL_BUTTON_MIDDLE)
	{
	}
	// Single mouse 4 click
	if (buttonDown == SDL_BUTTON_X1)
	{
	}
	// Single mouse 5 click
	if (buttonDown == SDL_BUTTON_X2)
	{
	}

	Uint8 mouse_state = SDL_GetMouseState(NULL, NULL);
	// Left click hold
	if (mouse_state & SDL_BUTTON_LMASK)
	{
	}
	// Right click hold
	if (mouse_state & SDL_BUTTON_RMASK)
	{
	}
	// Scroll click hold
	if (mouse_state & SDL_BUTTON_MMASK)
	{
	}
	// Mouse 4 click hold
	if (mouse_state & SDL_BUTTON_X1MASK)
	{
	}
	// Mouse 5 click hold
	if (mouse_state & SDL_BUTTON_X2MASK)
	{
	}

	// Left click release
	if (buttonUp == SDL_BUTTON_LEFT)
	{
	}
	// Right click release
	if (buttonUp == SDL_BUTTON_RIGHT)
	{
	}
	// Scroll click release
	if (buttonUp == SDL_BUTTON_MIDDLE)
	{
	}
	// Mouse 4 click release
	if (buttonUp == SDL_BUTTON_X1)
	{
	}
	// Mouse 5 click release
	if (buttonUp == SDL_BUTTON_X2)
	{
	}

	// Scroll wheel up
	if (scroll >= 1)
	{
	}
	// Scroll wheel down
	if (scroll <= -1)
	{
	}

	if (mMouseCaptured == SDL_TRUE)
	{
		// Calculate mouse movement
		int x = 0;
		int y = 0;
		SDL_GetRelativeMouseState(&x, &y);
		mMousePosX = x * MOUSE_SENSITIVITY;
		mMousePosY = -y * MOUSE_SENSITIVITY;
	}
}

void Game::Update(float deltaTime)
{
	PROFILE_SCOPE(UPDATE);

	mCamera->Update(deltaTime, mMousePosX, mMousePosY);

	mTimer += deltaTime;

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, mTimer, glm::vec3(0.0f, 0.0f, 1.0f));
	mModel = trans;

	for (auto e : mEntities)
	{
		e->Update(deltaTime);
	}
}

void Game::Render()
{
	PROFILE_SCOPE(RENDER);

	mCamera->SetActive();

	mRenderer->ClearBuffers();

	mShader->SetActive();
	mShader->SetMat4("model", mModel);

	mWallTexture->BindTexture();

	mVertexBuffer->Draw();

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
