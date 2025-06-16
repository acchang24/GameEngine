#include "Laser.h"
#include <iostream>
#include <glm/glm.hpp>
#include "Audio/AudioSystem.h"
#include "Components/CollisionComponent.h"
#include "Components/MoveComponent2D.h"
#include "Components/SpriteComponent.h"
#include "Graphics/Texture.h"
#include "MemoryManager/AssetManager.h"
#include "Physics/Physics.h"
#include "Util/Logger.h"
#include "Asteroid.h"
#include "Game.h"

Laser::Laser(Renderer2D* renderer, Game* game) :
	Entity2D(),
	mLaserSprite(new SpriteComponent(this, renderer)),
	mLaserMovement(new MoveComponent2D(this)),
	mBox(new OBBComponent2D(this, game->GetPhysics(), BodyType::Intersect)),
	mGame(game),
	mLaserDecay(0.0f)
{
	// Add and set laser sprite texture
	Texture* laserSprite = AssetManager::LoadTexture("Assets/Laser.png", TextureType::Sprite);
	mLaserSprite->AddSprite(laserSprite);
	mLaserSprite->SetSprite(laserSprite);

	// Set the entity size to the sprite size
	mSize = glm::vec2(laserSprite->GetWidth(), laserSprite->GetHeight());

	// Set laser speed
	mLaserMovement->SetMovementSpeed(1000.0f);

	// Set laser hit box size
	mBox->SetBoxSize(glm::vec2(30.0f, 10.0f));

	// Set on collision callback
	mBox->SetOnCollision([this](Entity2D* other, const CollisionResult& result) {
		// If collided with asteroid, destroy the asteroid and this laser
		Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
		if (asteroid)
		{
			mGame->GetAudio()->PlaySFX("Assets/Sounds/AsteroidExplode.wav");
			asteroid->SetEntityState(EntityState::Destroy);
			mState = EntityState::Destroy;
			mGame->GetLogger()->Log("Laser hit Asteroid", LogLevel::Info);
		}
	});

	// Add to game
	game->AddGameEntity(this);
}

Laser::~Laser()
{
	mGame->GetLogger()->Log("Deleted Laser", LogLevel::Info);
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
