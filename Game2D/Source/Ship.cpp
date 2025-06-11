#include "Ship.h"
#include "Audio/AudioSystem.h"
#include "Components/CollisionComponent.h"
#include "Components/MoveComponent2D.h"
#include "Components/SpriteComponent.h"
#include "Graphics/SpriteRenderer.h"
#include "Graphics/Texture.h"
#include "Input/Keyboard.h"
#include "MemoryManager/AssetManager.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(SpriteRenderer* renderer, Game* game) :
	Entity2D(),
	mSprite(new SpriteComponent(this, renderer)),
	mMovement(new MoveComponent2D(this)),
	mCollisionBox(new AABBComponent2D(this, game->GetPhysics())),
	mRenderer(renderer),
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

	// Set ship hit box size
	mCollisionBox->SetBoxSize(glm::vec2(100.0f, 90.0f));

	// Fire off loop sfx so this sound chunk can pause/resume later
	mGame->GetAudio()->PlaySFX("Assets/Sounds/ShipThrust.wav", -1, -1);
	// Pause sound immediately
	mGame->GetAudio()->PauseSFX("Assets/Sounds/ShipThrust.wav");

	// Add to game
	game->AddGameEntity(this);
}

Ship::~Ship()
{
	std::cout << "Deleted Ship\n";
}

void Ship::OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse)
{
	// Reset movement speed
	float moveSpeed = 0.0f;

	if (keyState[SDL_SCANCODE_W])
	{
		if (mGame->GetKeyboard()->HasLeadingEdge(keyState, SDL_SCANCODE_W))
		{
			mGame->GetAudio()->ResumeSFX("Assets/Sounds/ShipThrust.wav");
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
		mGame->GetAudio()->PauseSFX("Assets/Sounds/ShipThrust.wav");
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
		mGame->GetAudio()->PlaySFX("Assets/Sounds/Shoot.wav");

		Laser* laser = new Laser(mRenderer, mGame);
		laser->SetPosition(mPosition);
		laser->SetRotation(mRotation);

		mLaserCooldown = 0.0f;
	}

	if (mMovement)
	{
		mMovement->SetMovementSpeed(moveSpeed);
		mMovement->SetRotationSpeed(rotationSpeed);
	}

	mGame->GetKeyboard()->SavePrevKeyState(keyState, SDL_SCANCODE_W);
}

void Ship::OnUpdate(float deltaTime)
{
	// Update laser cool down
	mLaserCooldown += deltaTime;

	// Wrap screen if out of bounds
	if (mPosition.x < 0.0f)
	{
		mPosition.x = mRenderer->GetWidth();
	}
	if (mPosition.x > mRenderer->GetWidth())
	{
		mPosition.x = 0.0f;
	}
	if (mPosition.y < 0.0f)
	{
		mPosition.y = mRenderer->GetHeight();
	}
	if (mPosition.y > mRenderer->GetHeight())
	{
		mPosition.y = 0.0f;
	}
}
