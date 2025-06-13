#include "Asteroid.h"
#include "Components/MoveComponent2D.h"
#include "Components/CollisionComponent.h"
#include "Components/SpriteComponent.h"
#include "Graphics/Texture.h"
#include "MemoryManager/AssetManager.h"
#include "Util/Random.h"
#include "Game.h"

Asteroid::Asteroid(SpriteRenderer* renderer, Game* game) :
	Entity2D(),
	mSprite(new SpriteComponent(this, renderer)),
	mMovement(new MoveComponent2D(this)),
	mCollisionCircle(nullptr),
	mRenderer(renderer)
{
	// Add and set asteroid sprite texture
	Texture* asteroidSprite = AssetManager::LoadTexture("Assets/Asteroid.png", TextureType::Sprite);
	mSprite->AddSprite(asteroidSprite);
	mSprite->SetSprite(asteroidSprite);
	
	// Set entity size to size of asteroid sprite
	mSize = glm::vec2(asteroidSprite->GetWidth(), asteroidSprite->GetHeight());
	
	// Set a random movement speed between 50 and 150
	mMovement->SetMovementSpeed(0.0f);
	
	// Get random rotation degree
	mRotation = Random::GetFloatRange(0.0f, 360.0f);

	// Get random position
	//mPosition = Random::GetVector2(glm::vec2(0.0f, 0.0f), glm::vec2(renderer->GetWidth(), renderer->GetHeight()));

	mPosition = glm::vec2(400.0f, 400.0f);

	AABBComponent2D* collision = new AABBComponent2D(this, game->GetPhysics(), BodyType::Static);
	collision->SetBoxSize(mSize);

	mCollisionCircle = collision;

	// Set the collision circle radius
	//mCollisionCircle = new CircleComponent(this, game->GetPhysics(), asteroidSprite->GetWidth() * 0.5f, BodyType::Static);

	// Set the on collision callback
	mCollisionCircle->SetOnCollision([this](Entity2D* other, const CollisionResult& result) {
		// If collided with another asteroid, create a new rotation
		Asteroid* asteroid = dynamic_cast<Asteroid*>(other);
		if (asteroid)
		{
			SetRotation(GetRotation() + Random::GetFloatRange(75.0f, 90.0f));
		}
	});

	// Add to game
	game->AddGameEntity(this);
}

Asteroid::~Asteroid()
{
	std::cout << "Deleted Asteroid\n";
}

void Asteroid::OnUpdate(float deltaTime)
{
	// Wrap the screen if asteroid goes out of bounds
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

