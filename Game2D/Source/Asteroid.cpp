#include "Asteroid.h"
#include "Components/MoveComponent2D.h"
#include "Components/CollisionComponent.h"
#include "Components/SpriteComponent.h"
#include "Graphics/SpriteRenderer.h"
#include "Graphics/Texture.h"
#include "MemoryManager/AssetManager.h"
#include "Util/Random.h"
#include "Game.h"

Asteroid::Asteroid(SpriteRenderer* renderer, Game* game) :
	Entity2D(),
	mMovement(new MoveComponent2D(this)),
	mSprite(new SpriteComponent(this, renderer)),
	mCollisionCircle(nullptr),
	mRenderer(renderer)
{
	mMovement->SetMovementSpeed(Random::GetFloatRange(50.0f, 150.0f));
	
	// Get random rotation degree
	mRotation = Random::GetFloatRange(0.0f, 360.0f);
	// Get random position
	mPosition = Random::GetVector2(glm::vec2(0.0f, 0.0f), glm::vec2(renderer->GetWidth(), renderer->GetHeight()));

	Texture* asteroidSprite = AssetManager::LoadTexture("Assets/Asteroid.png", TextureType::Sprite);
	mSprite->AddSprite(asteroidSprite);
	mSprite->SetSprite(asteroidSprite);

	mSize = glm::vec2(asteroidSprite->GetWidth(), asteroidSprite->GetHeight());

	mCollisionCircle = new CircleComponent(this, game->GetPhysics(), asteroidSprite->GetWidth() * 0.5f);

	// Set the on collision callback
	mCollisionCircle->SetOnCollision([this](Entity2D* other, const CollisionResult& result) {
		// If collided with another asteroid, create a new rotation
		Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
		if (asteroid)
		{
			SetRotation(GetRotation() + Random::GetFloatRange(75.0f, 90.0f));
		}
	});

	game->AddGameEntity(this);
}

Asteroid::~Asteroid()
{
	std::cout << "Deleted Asteroid\n";
}

void Asteroid::OnUpdate(float deltaTime)
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
}

