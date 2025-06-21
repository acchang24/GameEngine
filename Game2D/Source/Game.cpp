#include "Game.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Audio/Sound.h"
#include "Components/SpriteComponent.h"
#include "EngineUI/EngineUI.h"
#include "Entity/Entity2D.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/Text.h"
#include "Graphics/Texture.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"
#include "Physics/Physics.h"
#include "Util/Console.h"
#include "Util/LoggerMacros.h"
#include "Util/Profiler.h"
#include "Util/Random.h"
#include "Asteroid.h"
#include "Ship.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
int SUB_SAMPLES = 4;
int VSYNC = 1;
double MOUSE_SENSITIVITY = 0.05;
bool IS_FULLSCREEN = false;
const char* TITLE = "Game2D";
SDL_bool MOUSE_CAPTURED = SDL_FALSE;

Game::Game() :
	mEngine(RendererMode::MODE_2D, MOUSE_SENSITIVITY),
	mIsRunning(true)
{
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	if (!mEngine.Init(WINDOW_WIDTH, WINDOW_HEIGHT, SUB_SAMPLES, VSYNC, IS_FULLSCREEN, MOUSE_CAPTURED, TITLE))
	{
		return false;
	}

	PROFILE_SCOPE(LOAD_DATA);

	LoadShaders();

	LoadGameData();

	return true;
}

void Game::Shutdown()
{
	UnloadGameData();

	mEngine.Shutdown();
}

void Game::LoadShaders() const
{
	AssetManager::LoadShader("sprite", "Shaders/sprite.vert", "Shaders/sprite.frag");
	AssetManager::LoadShader("text", "Shaders/text.vert", "Shaders/text.frag");
	AssetManager::LoadShader("uiBox", "Shaders/uiBox.vert", "Shaders/uiBox.frag");
}

void Game::LoadGameData()
{
	Renderer2D* renderer2D = mEngine.GetRenderer()->GetRenderer2D();
	Ship* ship = new Ship(renderer2D, this);
	ship->SetPosition(glm::vec2(200.0f, 200.0f));
	ship->SetRotation(45.0f);

	// Load 10 asteroids
	for (int i = 0; i < 10; ++i)
	{
		Asteroid* a = new Asteroid(renderer2D, this);
	}

	Music* music = AssetManager::LoadMusic("Assets/Sounds/AllTheThingsYouAre.mp3");
	music->SetVolume(90);
	music->Play(-1);

	// Background
	Entity2D* background = new Entity2D(static_cast<float>(mEngine.GetRenderer()->GetWidth()), static_cast<float>(mEngine.GetRenderer()->GetHeight()));
	background->SetPosition(glm::vec2(static_cast<float>(mEngine.GetRenderer()->GetWidth() / 2), static_cast<float>(mEngine.GetRenderer()->GetHeight() / 2)));
	SpriteComponent* backgroundSC = new SpriteComponent(background, renderer2D, 50);
	backgroundSC->AddSprite(AssetManager::LoadTexture("Assets/Stars.png", TextureType::Sprite));
	backgroundSC->SetSprite(backgroundSC->GetSprite("Assets/Stars.png"));
	AddGameEntity(background);

	// Set 2d renderer shader
	renderer2D->SetSpriteShader(AssetManager::LoadShader("sprite"));
	renderer2D->SetTextShader(AssetManager::LoadShader("text"));
	renderer2D->SetUIBoxShader(AssetManager::LoadShader("uiBox"));

	// Set font
	mEngine.GetRenderer()->GetRenderer2D()->GetTextRenderer()->LoadFont("Assets/Fonts/arial.ttf", 16);
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
	Mouse* mouse = mEngine.GetMouse();
	Keyboard* keyboard = mEngine.GetKeyboard();

	mouse->ResetState();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		mEngine.GetEngineUI()->ProcessSDLEvent(event);
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
			mouse->SetButtonDown(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			// Mouse click release
			mouse->SetButtonUp(event.button.button);
			break;
		case SDL_MOUSEWHEEL:
			// Mouse wheel scroll
			mouse->SetScrollDir(event.wheel.y);
			break;
		}
	}

	ProcessMouseInput(mouse);

	const Uint8* keyboardState = keyboard->GetState();

	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (size_t i =0; i<mEntities.size(); ++i)
	{
		mEntities[i]->ProcessInput(keyboardState, keyboard, mouse);
	}

	mEngine.GetConsole()->ProcessInput(keyboardState, keyboard, mouse);
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

	if (mouse->MouseCaptured() == SDL_TRUE)
	{
		mouse->CalculateMovement();
	}
}

void Game::Update(float deltaTime)
{
	PROFILE_SCOPE(UPDATE);

	std::vector<Entity*> destroy;

	for (size_t i = 0; i < mEntities.size(); ++i)
	{
		Entity* e = mEntities[i];
		e->Update(deltaTime);
		if (e->GetEntityState() == EntityState::Destroy)
		{
			destroy.emplace_back(e);
		}
	}

	for (auto d : destroy)
	{
		RemoveGameEntity(d);
	}

	mEngine.GetPhysics()->Update(deltaTime);

	PROFILE_SCOPE(WAIT_JOBS);
	JobManager::Get()->WaitForJobs();
}

void Game::Render()
{
	PROFILE_SCOPE(RENDER);

	mEngine.GetEngineUI()->SetUI();

	Renderer* renderer = mEngine.GetRenderer();

	renderer->ClearBuffers();

	renderer->Draw2D();

	mEngine.GetEngineUI()->Render();

	renderer->EndFrame();
}

void Game::ResizeWindow(const SDL_Event& event)
{
	SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
	if (window)
	{
		int width = event.window.data1;
		int height = event.window.data2;

		float ratio = static_cast<float>(width) / static_cast<float>(height);


		// Set new viewport dimensions
		glViewport(0, 0, width, height);

		printf("Window width: %i, Window height: %i\n", width, height);

		Renderer* renderer = mEngine.GetRenderer();

		// Set new window dimensions
		renderer->SetWidth(width);
		renderer->SetHeight(height);

		// Resize frame buffers
		renderer->ResizeFrameBuffers();
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
