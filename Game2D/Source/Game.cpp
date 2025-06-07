#include "Game.h"
#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Components/SpriteComponent.h"
#include "Entity/Entity.h"
#include "Graphics/Texture.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"
#include "Profiler/Profiler.h"
#include "Util/Random.h"


const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
int SUB_SAMPLES = 4;
int VSYNC = 1;
double MOUSE_SENSITIVITY = 0.05;
bool IS_FULLSCREEN = false;
const char* TITLE = "Game2D";
SDL_bool MOUSE_CAPTURED = SDL_FALSE;

Game::Game() :
	mRenderer(RendererMode::MODE_2D),
	mAssetManager(AssetManager::Get()),
	mJobManager(JobManager::Get()),
	mCamera(nullptr),
	mMouse(MOUSE_SENSITIVITY, MOUSE_CAPTURED),
	mKeyboard(),
	mIsRunning(true)
{
	mAssetManager->SetRenderer(&mRenderer);
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	mJobManager->Begin();

	if (!mRenderer.Init(WINDOW_WIDTH, WINDOW_HEIGHT, SUB_SAMPLES, VSYNC, IS_FULLSCREEN, MOUSE_CAPTURED, TITLE))
	{
		return false;
	}

	mCamera = new Camera(&mRenderer);
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	PROFILE_SCOPE(LOAD_DATA);

	LoadShaders();

	LoadGameData();

	return true;
}

void Game::Shutdown()
{
	UnloadGameData();

	delete mCamera;

	mRenderer.Shutdown();

	mJobManager->End();

	mAssetManager->Shutdown();
}

void Game::LoadShaders() const
{
	AssetManager::LoadShader("sprite", "Shaders/sprite.vert", "Shaders/sprite.frag");
}

void Game::LoadGameData()
{
	Entity* face = new Entity();
	face->SetPos2D(glm::vec2(200.0f, 200.0f));
	face->SetRotation(45.0f);
	face->SetSize(glm::vec2(300.0f, 400.0f));
	Texture* sprite = AssetManager::LoadTexture("Assets/awesomeface.png", TextureType::Sprite);
	SpriteComponent* sc = new SpriteComponent(face, mRenderer.GetSpriteRenderer());
	sc->AddSprite(sprite);
	sc->SetSprite(sc->GetSprite("Assets/awesomeface.png"));
	AddGameEntity(face);

	mRenderer.GetSpriteRenderer()->SetShader(AssetManager::LoadShader("sprite"));
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

		std::cout << mEntities[0]->GetPos2D().x << " " << mEntities[0]->GetPos2D().y << " " << mEntities[0]->GetRotation() << "\n";

		ProcessInput();

		Update(deltaTime);

		Render();
	}
}

void Game::ProcessInput()
{
	mMouse.ResetState();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			// Check to see when user quits the game
			mIsRunning = false;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ResizeWindow(event);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			// Mouse click down
			mMouse.SetButtonDown(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			// Mouse click release
			mMouse.SetButtonUp(event.button.button);
			break;
		case SDL_MOUSEWHEEL:
			// Mouse wheel scroll
			mMouse.SetScrollDir(event.wheel.y);
			break;
		}
	}

	ProcessMouseInput(&mMouse);

	const Uint8* keyboardState = mKeyboard.GetState();

	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}


	// Move camera around
	if (keyboardState[SDL_SCANCODE_W])
	{
		Entity* e = mEntities[0];
		e->SetPos2D(e->GetPos2D() + glm::vec2(0.0f, -1.0f) * 0.5f);
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		Entity* e = mEntities[0];
		e->SetPos2D(e->GetPos2D() + glm::vec2(0.0f, 1.0f) * 0.5f);
	}
	if (keyboardState[SDL_SCANCODE_A])
	{
		Entity* e = mEntities[0];
		e->SetPos2D(e->GetPos2D() + glm::vec2(-1.0f, 0.0f) * 0.5f);
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		Entity* e = mEntities[0];
		e->SetPos2D(e->GetPos2D() + glm::vec2(1.0f, 0.0f) * 0.5f);
	}
	if (keyboardState[SDL_SCANCODE_RIGHT])
	{
		Entity* e = mEntities[0];
		e->SetRotation(e->GetRotation() + 0.5f);
	}
	if (keyboardState[SDL_SCANCODE_LEFT])
	{
		Entity* e = mEntities[0];
		e->SetRotation(e->GetRotation() - 0.5f);
	}


	// Toggle camera modes
	if (mKeyboard.HasLeadingEdge(keyboardState, SDL_SCANCODE_V))
	{
		mCamera->ToggleCameraModes();
	}

	// Capture mouse
	if (mKeyboard.HasLeadingEdge(keyboardState, SDL_SCANCODE_M))
	{
		mMouse.ToggleMouseCapture();
	}

	mKeyboard.SavePrevKeyState(keyboardState, SDL_SCANCODE_M);
	mKeyboard.SavePrevKeyState(keyboardState, SDL_SCANCODE_V);

	for (auto e : mEntities)
	{
		e->ProcessInput(keyboardState, &mKeyboard, &mMouse);
	}
}

void Game::ProcessMouseInput(Mouse* mouse)
{
	Uint8 buttonDown = mouse->GetButtonDown();
	Uint8 buttonUp = mouse->GetButtonUp();
	Sint32 scroll = mouse->GetScrollDir();
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

	if (mMouse.MouseCaptured() == SDL_TRUE)
	{
		mMouse.CalculateMovement();
	}
}

void Game::Update(float deltaTime)
{
	PROFILE_SCOPE(UPDATE);

	for (auto e : mEntities)
	{
		e->Update(deltaTime);
	}

	mCamera->Update(deltaTime, &mMouse);

	PROFILE_SCOPE(WAIT_JOBS);
	mJobManager->WaitForJobs();
}

void Game::Render()
{
	PROFILE_SCOPE(RENDER);

	mCamera->SetBuffer();

	mRenderer.ClearBuffers();

	mRenderer.Draw2D();

	mRenderer.EndFrame();
}

void Game::ResizeWindow(const SDL_Event& event)
{
	SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
	if (window)
	{
		int width = event.window.data1;
		int height = event.window.data2;

		float ratio = static_cast<float>(width) / static_cast<float>(height);

		// Set new camera aspect ratio
		mCamera->SetAspectRatio(ratio);

		// Set new viewport dimensions
		glViewport(0, 0, width, height);

		printf("Window width: %i, Window height: %i\n", width, height);

		// Set new window dimensions
		mRenderer.SetWidth(width);
		mRenderer.SetHeight(height);

		// Resize frame buffers
		mRenderer.ResizeFrameBuffers();
	}
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
