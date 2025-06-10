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
	mLaserDecay(0.0f)
{
	mLaserMovement->SetMovementSpeed(1000.0f);

	Texture* laserSprite = AssetManager::LoadTexture("Assets/Laser.png", TextureType::Sprite);
	mLaserSprite->AddSprite(laserSprite);
	mLaserSprite->SetSprite(laserSprite);

	mSize = glm::vec2(laserSprite->GetWidth(), laserSprite->GetHeight());

	game->AddGameEntity(this);

	mBox->SetBoxSize(glm::vec2(30.0f, 10.0f));

	// Set on collision callback
	mBox->SetOnCollision([this](Entity2D* other, const CollisionResult& result) {
		// If collided with asteroid, destroy the asteroid and this laser
		Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
		if (asteroid)
		{
			asteroid->SetEntityState(EntityState::Destroy);
			mState = EntityState::Destroy;
		}
	});
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
}
