#include "Laser.h"
#include <iostream>
#include <glm/glm.hpp>
#include "Components/SpriteComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/MoveComponent2D.h"
#include "Graphics/Texture.h"
#include "MemoryManager/AssetManager.h"
#include "Game.h"
#include "Asteroid.h"
#include "Physics/Physics.h"

Laser::Laser(SpriteRenderer* renderer, Game* game) :
	Entity2D(),
	mLaserSprite(new SpriteComponent(this, renderer)),
	mLaserMovement(new MoveComponent2D(this)),
	mRenderer(renderer),
	mBox(new AABBComponent2D(this, game->GetPhysics(), BodyType::Intersect)),
	mGame(game),
	mLaserDecay(0.0f)
{
	mLaserMovement->SetMovementSpeed(900.0f);

	Texture* laserSprite = AssetManager::LoadTexture("Assets/Laser.png", TextureType::Sprite);
	mLaserSprite->AddSprite(laserSprite);
	mLaserSprite->SetSprite(laserSprite);

	mSize = glm::vec2(laserSprite->GetWidth(), laserSprite->GetHeight());

	game->AddGameEntity(this);

	mBox->SetBoxSize(glm::vec2(30.0f, 10.0f));
}

Laser::~Laser()
{
	std::cout << "Deleted Laser\n";
}

void Laser::OnUpdate(float deltaTime)
{
	mLaserDecay += deltaTime;

	if (mLaserDecay >= 1.0f)
	{
		mState = EntityState::Destroy;

		mLaserDecay = 0.0f;
	}

	const std::vector<Asteroid*>& asteroids = mGame->GetAsteroids();

	for (auto a : asteroids)
	{
		float dist = glm::length(a->GetPosition() - mPosition);
		glm::vec2 offset(0.0f, 0.0f);
		
		if (Physics::IntersectCircleVsAABB2D(a->GetComponent<CircleComponent>(), mBox, offset))
		{
			mState = EntityState::Destroy;
			a->SetEntityState(EntityState::Destroy);
		}
	}
}
