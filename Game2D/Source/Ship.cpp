#include "Ship.h"
#include "Audio/AudioSystem.h"
#include "Components/CollisionComponent.h"
#include "Components/MoveComponent2D.h"
#include "Components/SpriteComponent.h"
#include "Graphics/Texture.h"
#include "Input/Keyboard.h"
#include "MemoryManager/AssetManager.h"
#include "Util/LoggerMacros.h"
#include "Engine.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game) :
	Entity2D(),
	mSprite(new SpriteComponent(this, game->GetEngine()->GetRenderer()->GetRenderer2D())),
	mMovement(new MoveComponent2D(this)),
	mCollisionBox(nullptr),
	mEngine(game->GetEngine()),
	mGame(game),
	mLaserCooldown(1.0f)
{
	// Load and set ship sprites
	Texture* shipSprite = AssetManager::LoadTexture("Assets/Ship.png", TextureType::Sprite);
	mSprite->AddSprite(AssetManager::LoadTexture("Assets/ShipThrust.png", TextureType::Sprite));
	mSprite->AddSprite(shipSprite);
	mSprite->SetSprite(shipSprite);

	// Set the entity size to the ship sprite size
	mSize = glm::vec2(shipSprite->GetWidth(), shipSprite->GetHeight());

	// Create an oriented bounded box for the ship's collision box
	OBBComponent2D* box = new OBBComponent2D(this, mEngine->GetPhysics());
	box->SetBoxSize(glm::vec2(100.0f, 90.0f));

	mCollisionBox = box;

	// Fire off loop sfx so this sound chunk can pause/resume later
	mEngine->GetAudio()->PlaySFX("Assets/Sounds/ShipThrust.wav", -1, -1);
	// Pause sound immediately
	mEngine->GetAudio()->PauseSFX("Assets/Sounds/ShipThrust.wav");

	// Add to game
	game->AddGameEntity(this);
}

Ship::~Ship()
{
	LOG_DEBUG("Deleted Ship");
}

void Ship::OnProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse)
{
	// Reset movement speed
	float moveSpeed = 0.0f;

	if (keyState[SDL_SCANCODE_W])
	{
		if (mEngine->GetKeyboard()->HasLeadingEdge(keyState, SDL_SCANCODE_W))
		{
			mEngine->GetAudio()->ResumeSFX("Assets/Sounds/ShipThrust.wav");
		}
		moveSpeed += 200.0f;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		moveSpeed -= 40.0f;
	}

	if (moveSpeed > 0.0f)
	{
		mSprite->SetSprite(mSprite->GetSprite("Assets/ShipThrust.png"));
	}
	else
	{
		mSprite->SetSprite(mSprite->GetSprite("Assets/Ship.png"));
		mEngine->GetAudio()->PauseSFX("Assets/Sounds/ShipThrust.wav");
	}

	float rotationSpeed = 0.0f;
	if (keyState[SDL_SCANCODE_A])
	{
		rotationSpeed -= 100.0f;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		rotationSpeed += 100.0f;
	}

	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown > 1.0f)
	{
		mEngine->GetAudio()->PlaySFX("Assets/Sounds/Shoot.wav");

		Laser* laser = new Laser(mGame);
		laser->SetPosition(mPosition);
		laser->SetRotation(mRotation);

		mLaserCooldown = 0.0f;
	}

	if (mMovement)
	{
		mMovement->SetMovementSpeed(moveSpeed);
		mMovement->SetRotationSpeed(rotationSpeed);
	}

	keyboard->SavePrevKeyState(keyState, SDL_SCANCODE_W);
}

void Ship::OnUpdate(float deltaTime)
{
	// Update laser cool down
	mLaserCooldown += deltaTime;

	// Wrap screen if out of bounds
	if (mPosition.x < 0.0f)
	{
		mPosition.x = mEngine->GetRenderer()->GetWidth();
	}
	if (mPosition.x > mEngine->GetRenderer()->GetWidth())
	{
		mPosition.x = 0.0f;
	}
	if (mPosition.y < 0.0f)
	{
		mPosition.y = mEngine->GetRenderer()->GetHeight();
	}
	if (mPosition.y > mEngine->GetRenderer()->GetHeight())
	{
		mPosition.y = 0.0f;
	}
}
