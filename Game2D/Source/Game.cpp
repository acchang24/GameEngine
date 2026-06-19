#include "Game.h"
#include <algorithm>
#include <chrono>
#include <glm/glm.hpp>
#include "Audio/Sound.h"
#include "Components/CollisionComponent.h"
#include "Components/MoveComponent2D.h"
#include "Components/SpriteComponent.h"
#include "EngineUI/EngineUI.h"
#include "Entity/Entity.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/Text.h"
#include "Graphics/Texture.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"
#include "Physics/Physics.h"
#include "Util/Logger.h"
#include "Util/Profiler.h"
#include "Util/Random.h"
#include "EngineContext.h"
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
	mEngine(RendererMode::MODE_2D),
	mConsole(),
	mBackground(nullptr),
	mIsRunning(true)
{
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	if (!mEngine.Init(WINDOW_WIDTH, WINDOW_HEIGHT, SUB_SAMPLES, VSYNC, IS_FULLSCREEN, MOUSE_SENSITIVITY, MOUSE_CAPTURED, TITLE))
	{
		return false;
	}

	const EngineContext& engineContext = mEngine.GetContext();

	AssetManager* assetManager = engineContext.assetManager;

	PROFILE_SCOPE(LOAD_DATA);

	LoadShaders(assetManager);

	LoadAssets(assetManager);

	LoadGameData(engineContext);

	return true;
}

void Game::Shutdown()
{
	UnloadGameData();

	mEngine.Shutdown();
}

void Game::LoadShaders(AssetManager* assetManager) const
{
	assetManager->LoadShader("sprite", "Shaders/sprite.vert", "Shaders/sprite.frag");
	assetManager->LoadShader("text", "Shaders/text.vert", "Shaders/text.frag");
	assetManager->LoadShader("uiBox", "Shaders/uiBox.vert", "Shaders/uiBox.frag");
}

void Game::LoadAssets(AssetManager* assetManager) const
{
	assetManager->LoadTexture("Assets/Ship.png", TextureType::Sprite);
	assetManager->LoadTexture("Assets/ShipThrust.png", TextureType::Sprite);
	assetManager->LoadTexture("Assets/Laser.png", TextureType::Sprite);
	assetManager->LoadTexture("Assets/Asteroid.png", TextureType::Sprite);
	assetManager->LoadTexture("Assets/Stars.png", TextureType::Sprite);
	assetManager->LoadSFX("Assets/Sounds/ShipThrust.wav");
	assetManager->LoadSFX("Assets/Sounds/Shoot.wav");
	assetManager->LoadSFX("Assets/Sounds/AsteroidExplode.wav");
	assetManager->LoadMusic("Assets/Sounds/AllTheThingsYouAre.mp3");
}

void Game::LoadGameData(const EngineContext& engineContext)
{
	AssetManager* assetManager = engineContext.assetManager;

	Ship* ship = new Ship(this);
	ship->SetPosition2D(glm::vec2(200.0f, 200.0f));

	// Set ship sprite component
	SpriteComponent* shipSpriteComp = new SpriteComponent(ship, engineContext.renderer->GetRenderer2D());
	// Load texture from cache
	Texture* shipSprite = assetManager->LoadTexture("Assets/Ship.png");
	shipSpriteComp->AddSprite(shipSprite);
	// Load second texture from cache
	shipSpriteComp->AddSprite(assetManager->LoadTexture("Assets/ShipThrust.png"));
	// Set the sprite
	shipSpriteComp->SetSprite(shipSprite);
	ship->SetSpriteComp(shipSpriteComp);

	// Ship movement component
	MoveComponent2D* shipMove = new MoveComponent2D(ship);
	ship->SetMoveComp(shipMove);

	// Ship hit collision component
	OBBComponent2D* shipHitBox = new OBBComponent2D(ship, engineContext.physics);
	shipHitBox->SetBoxSize(glm::vec2(100.0f, 90.0f));
	ship->SetCollisionComp(shipHitBox);

	// Fire off loop sfx so this sound chunk can pause/resume later
	engineContext.audio->PlaySFX(assetManager->LoadSFX("Assets/Sounds/ShipThrust.wav"), -1, -1);
	// Pause sound immediately
	engineContext.audio->PauseSFX(assetManager->LoadSFX("Assets/Sounds/ShipThrust.wav"));


	// Load 10 asteroids
	for (int i = 0; i < 10; ++i)
	{
		Asteroid* asteroid = new Asteroid(this);
		
		// Set random position
		asteroid->SetPosition2D(Random::GetVector2(glm::vec2(0.0f, 0.0f), glm::vec2(engineContext.renderer->GetWidth(), engineContext.renderer->GetHeight())));
		
		// Set random rotation
		asteroid->SetRotation2D(glm::angleAxis(glm::radians(Random::GetFloatRange(0.0f, 360.0f)), glm::vec3(0.0f, 0.0f, 1.0f)));

		// Asteroid sprite component
		SpriteComponent* asteroidSpriteComp = new SpriteComponent(asteroid, engineContext.renderer->GetRenderer2D());
		Texture* asteroidSprite = assetManager->LoadTexture("Assets/Asteroid.png");
		asteroidSpriteComp->AddSprite(asteroidSprite);
		asteroidSpriteComp->SetSprite(asteroidSprite);

		// Asteroid move component
		MoveComponent2D* asteroidMove = new MoveComponent2D(asteroid);
		// Set random move speed in range
		asteroidMove->SetMovementSpeed(Random::GetFloatRange(50.0f, 150.0f));

		// Asteroid collision component
		CircleComponent* asteroidColl = new CircleComponent(asteroid, engineContext.physics, asteroidSprite->GetWidth() * 0.5f);
		// Set on collide
		asteroidColl->SetOnCollision([asteroid](Entity* other, const CollisionResult& result) {
			// If collided with another asteroid, create a new rotation
			Asteroid* otherAsteroid = dynamic_cast<Asteroid*>(other);
			if (otherAsteroid)
			{
				asteroid->SetRotation2D(asteroid->GetQuatRotation() + glm::angleAxis(glm::radians(Random::GetFloatRange(0.0f, 360.0f)), glm::vec3(0.0f, 0.0f, 1.0f)));
			}
		});
	}

	Music* music = assetManager->LoadMusic("Assets/Sounds/AllTheThingsYouAre.mp3");
	music->SetVolume(90);
	music->Play(-1);

	Renderer* renderer = engineContext.renderer;

	Renderer2D* renderer2D = renderer->GetRenderer2D();

	// Background
	mBackground = new Entity();
	mBackground->SetPosition2D(glm::vec2(static_cast<float>(renderer->GetWidth() / 2), static_cast<float>(renderer->GetHeight() / 2)));
	SpriteComponent* backgroundSC = new SpriteComponent(mBackground, renderer2D, 50);
	backgroundSC->AddSprite(assetManager->LoadTexture("Assets/Stars.png", TextureType::Sprite));
	backgroundSC->SetSprite(backgroundSC->GetSprite("Assets/Stars.png"));
	// Sprite background resize
	SpriteComponent* bgSprite = mBackground->GetComponent<SpriteComponent>();
	if (bgSprite)
	{
		bgSprite->SetSize(glm::vec2(renderer->GetWidth(), renderer->GetHeight()));
	}
	AddGameEntity(mBackground);

	// Set 2d renderer shader
	renderer2D->SetSpriteShader(assetManager->LoadShader("sprite"));
	renderer2D->SetTextShader(assetManager->LoadShader("text"));
	renderer2D->SetUIBoxShader(assetManager->LoadShader("uiBox"));

	// Set font
	renderer2D->GetTextRenderer()->LoadFont("Assets/Fonts/arial.ttf", 16);
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

	const EngineContext& engineContext = mEngine.GetContext();

	while (mIsRunning)
	{
		Profiler::Get()->ResetAll();

		PROFILE_SCOPE(GAME_LOOP);

		// Calculate delta time
		std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
		double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count());
		float deltaTime = static_cast<float>(0.000000001 * duration);
		frameStart = frameEnd;

		ProcessInput(engineContext);

		Update(deltaTime, engineContext);

		Render(engineContext);
	}
}

void Game::ProcessInput(const EngineContext& engineContext)
{
	InputSystem* input = engineContext.input;

	input->StartFrame();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		input->HandleEvent(event);

		engineContext.engineUI->ProcessSDLEvent(event);

		switch (event.type)
		{
		case SDL_QUIT:
			// Check to see when user quits the game
			mIsRunning = false;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ResizeWindow(event, engineContext);
			}
			break;
		}
	}

	input->GetCurrentState();

	ProcessMouseInput(input);

	if (input->IsKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		mIsRunning = false;
	}

	for (size_t i =0; i<mEntities.size(); ++i)
	{
		mEntities[i]->ProcessInput(input, engineContext);
	}

	mConsole.ProcessInput(input);
}

void Game::ProcessMouseInput(InputSystem* input)
{
	Sint32 scroll = input->GetMouseScrollDir();

	// Single left click
	if (input->IsMouseSingleClick(SDL_BUTTON_LEFT))
	{
		LOG_DEBUG("Left click");
	}
	// Single right click
	if (input->IsMouseSingleClick(SDL_BUTTON_RIGHT))
	{
		LOG_DEBUG("Right click");
	}
	// Single scroll wheel click (mouse 3 click)
	if (input->IsMouseSingleClick(SDL_BUTTON_MIDDLE))
	{
		LOG_DEBUG("Scroll click");
	}
	// Single mouse 4 click
	if (input->IsMouseSingleClick(SDL_BUTTON_X1))
	{
		LOG_DEBUG("Mouse 4 click");
	}
	// Single mouse 5 click
	if (input->IsMouseSingleClick(SDL_BUTTON_X2))
	{
		LOG_DEBUG("Mouse 5 click");
	}


	// Left click hold
	if (input->IsMouseHeld(SDL_BUTTON_LEFT))
	{
		LOG_DEBUG("Left click hold");
	}
	// Right click hold
	if (input->IsMouseHeld(SDL_BUTTON_RIGHT))
	{
		LOG_DEBUG("Right click hold");
	}
	// Scroll click hold
	if (input->IsMouseHeld(SDL_BUTTON_MIDDLE))
	{
		LOG_DEBUG("Scroll click hold");
	}
	// Mouse 4 click hold
	if (input->IsMouseHeld(SDL_BUTTON_X1))
	{
		LOG_DEBUG("Mouse 4 click hold");
	}
	// Mouse 5 click hold
	if (input->IsMouseHeld(SDL_BUTTON_X2))
	{
		LOG_DEBUG("Mouse 5 click hold");
	}

	// Left click release
	if (input->IsMouseRelease(SDL_BUTTON_LEFT))
	{
		LOG_DEBUG("Left click release");
	}
	// Right click release
	if (input->IsMouseRelease(SDL_BUTTON_RIGHT))
	{
		LOG_DEBUG("Right click release");
	}
	// Scroll click release
	if (input->IsMouseRelease(SDL_BUTTON_MIDDLE))
	{
		LOG_DEBUG("Scroll click release");
	}
	// Mouse 4 click release
	if (input->IsMouseRelease(SDL_BUTTON_X1))
	{
		LOG_DEBUG("Mouse 4 click release");
	}
	// Mouse 5 click release
	if (input->IsMouseRelease(SDL_BUTTON_X2))
	{
		LOG_DEBUG("Mouse 5 click release");
	}

	// Scroll wheel up
	if (scroll >= 1)
	{
		LOG_DEBUG("Scroll up " + std::to_string(scroll));
	}
	// Scroll wheel down
	if (scroll <= -1)
	{
		LOG_DEBUG("Scroll down " + std::to_string(scroll));
	}
}

void Game::Update(float deltaTime, const EngineContext& engineContext)
{
	PROFILE_SCOPE(UPDATE);

	std::vector<Entity*> destroy;

	for (size_t i = 0; i < mEntities.size(); ++i)
	{
		Entity* e = mEntities[i];
		e->Update(deltaTime, engineContext);
		if (e->GetEntityState() == EntityState::Destroy)
		{
			destroy.emplace_back(e);
		}
	}

	for (auto d : destroy)
	{
		RemoveGameEntity(d);
	}

	engineContext.physics->Update(deltaTime);

	PROFILE_SCOPE(WAIT_JOBS);
	engineContext.jobManager->WaitForJobs();
}

void Game::Render(const EngineContext& engineContext)
{
	PROFILE_SCOPE(RENDER);

	Renderer* renderer = engineContext.renderer;

	EngineUI* ui = engineContext.engineUI;

	ui->SetUI();

	mConsole.SetConsoleUI(engineContext);

	engineContext.editor->SetEditorUI();

	renderer->ClearBuffers();

	renderer->Draw2D();

	ui->Render();

	renderer->EndFrame();
}

void Game::ResizeWindow(const SDL_Event& event, const EngineContext& engineContext)
{
	SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
	if (window)
	{
		int width = event.window.data1;
		int height = event.window.data2;

		engineContext.renderer->Resize(width, height);

		// Sprite background resize
		SpriteComponent* bgSprite = mBackground->GetComponent<SpriteComponent>();
		if (bgSprite)
		{
			bgSprite->SetSize(glm::vec2(width, height));
		}
		mBackground->SetPosition2D(glm::vec2(width / 2, height / 2));
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
