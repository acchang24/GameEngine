#include "Ship.h"
#include "Audio/AudioSystem.h"
#include "Components/MoveComponent2D.h"
#include "Components/SpriteComponent.h"
#include "Input/Keyboard.h"
#include "Util/Logger.h"
#include "Engine.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game) :
	Entity(),
	mSprite(nullptr),
	mMovement(nullptr),
	mCollisionBox(nullptr),
	mEngine(game->GetEngine()),
	mGame(game),
	mLaserCooldown(1.0f)
{
	game->AddGameEntity(this);
}

Ship::~Ship()
{
	LOG_DEBUG("Deleted Ship");
}

void Ship::OnProcessInput(const InputSystem* input, const EngineContext& engineContext)
{
	// Reset movement speed
	float moveSpeed = 0.0f;

	if (input->IsKeyPressed(SDL_SCANCODE_W))
	{
		if (input->IsKeyLeadingEdge(SDL_SCANCODE_W))
		{
			engineContext.audio->ResumeSFX(engineContext.assetManager->LoadSFX("Assets/Sounds/ShipThrust.wav"));
		}
		moveSpeed += 200.0f;
	}
	if (input->IsKeyPressed(SDL_SCANCODE_S))
	{
		moveSpeed -= 200.0f;
	}

	if (moveSpeed > 0.0f)
	{
		mSprite->SetSprite(mSprite->GetSprite("Assets/ShipThrust.png"));
	}
	else
	{
		mSprite->SetSprite(mSprite->GetSprite("Assets/Ship.png"));
		engineContext.audio->PauseSFX(engineContext.assetManager->LoadSFX("Assets/Sounds/ShipThrust.wav"));
	}

	float rotationSpeed = 0.0f;
	if (input->IsKeyPressed(SDL_SCANCODE_A))
	{
		rotationSpeed -= glm::pi<float>();
	}
	if (input->IsKeyPressed(SDL_SCANCODE_D))
	{
		rotationSpeed += glm::pi<float>();
	}

	if (input->IsKeyPressed(SDL_SCANCODE_SPACE) && mLaserCooldown > 1.0f)
	{
		engineContext.audio->PlaySFX(engineContext.assetManager->LoadSFX("Assets/Sounds/Shoot.wav"));

		Laser* laser = new Laser(mGame);
		laser->SetPosition2D(mPosition);
		laser->SetRotation2D(mRotation);

		mLaserCooldown = 0.0f;
	}

	if (mMovement)
	{
		mMovement->SetMovementSpeed(moveSpeed);
		mMovement->SetRotationSpeed(rotationSpeed);
	}
}

void Ship::OnUpdate(float deltaTime, const EngineContext& engineContext)
{
	// Update laser cool down
	mLaserCooldown += deltaTime;

	// Wrap screen if out of bounds
	if (mPosition.x < 0.0f)
	{
		mPosition.x = engineContext.renderer->GetWidth();
	}
	if (mPosition.x > engineContext.renderer->GetWidth())
	{
		mPosition.x = 0.0f;
	}
	if (mPosition.y < 0.0f)
	{
		mPosition.y = engineContext.renderer->GetHeight();
	}
	if (mPosition.y > engineContext.renderer->GetHeight())
	{
		mPosition.y = 0.0f;
	}
}
