#include "Ship.h"
#include "MemoryManager/AssetManager.h"
#include "Components/CollisionComponent.h"
#include "Components/MoveComponent2D.h"
#include "Components/SpriteComponent.h"
#include "Graphics/SpriteRenderer.h"
#include "Graphics/Texture.h"
#include "Game.h"

Ship::Ship(SpriteRenderer* renderer, Game* game, int test) :
	Entity2D(),
	mMovement(nullptr),
	mSprite(new SpriteComponent(this, renderer)),
	mCollisionBox(new AABBComponent2D(this)),
	mRenderer(renderer),
	mGame(game)
{
	if (test == 1)
	{
		mMovement = new MoveComponent2D(this);
	}

	game->AddGameEntity(this);

	Texture* shipSprite = AssetManager::LoadTexture("Assets/Ship.png", TextureType::Sprite);

	mSprite->AddSprite(shipSprite);
	mSprite->AddSprite(AssetManager::LoadTexture("Assets/ShipThrust.png", TextureType::Sprite));
	
	mSprite->SetSprite(shipSprite);

	mSize = glm::vec2(shipSprite->GetWidth(), shipSprite->GetHeight());

	mCollisionBox->SetBoxSize(glm::vec2(100.0f, 90.0f));
	game->AddCollision(mCollisionBox);
}

void Ship::OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse)
{
	mSprite->SetSprite(mSprite->GetSprite("Assets/Ship.png"));

	
	float moveSpeed = 0.0f;
	if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])
	{
		moveSpeed += 200.0f;
	}
	if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN])
	{
		moveSpeed -= 200.0f;
	}

	if (moveSpeed > 0.0f)
	{
		mSprite->SetSprite(mSprite->GetSprite("Assets/ShipThrust.png"));
	}

	float rotationSpeed = 0.0f;
	if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])
	{
		rotationSpeed -= 100.0f;
	}
	if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT])
	{
		rotationSpeed += 100.0f;
	}

	if (mMovement)
	{
		mMovement->SetMovementSpeed(moveSpeed);
		mMovement->SetRotationSpeed(rotationSpeed);
	}

}

void Ship::OnUpdate(float deltaTime)
{
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

	const std::vector<AABBComponent2D*>& collisions =  mGame->GetCollisions();

	for (auto c : collisions)
	{
		glm::vec2 offset(0.0f);

		if (c != mCollisionBox)
		{
			if (mMovement)
			{
				if (mMovement->GetMoveSpeed() > 0.0f)
				{
					CollisionSide side = mCollisionBox->GetMinOverlap(c, offset);

					mPosition += offset;
				}
			}
		}
	}
}
