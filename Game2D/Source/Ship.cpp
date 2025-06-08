#include "Ship.h"
#include "MemoryManager/AssetManager.h"
#include "Components/SpriteComponent.h"
#include "Graphics/SpriteRenderer.h"
#include "Graphics/Texture.h"
#include "Game.h"

Ship::Ship(SpriteRenderer* renderer, Game* game) :
	Entity2D(),
	mSpriteComponent(new SpriteComponent(this, renderer))
{
	game->AddGameEntity(this);

	Texture* shipSprite = AssetManager::LoadTexture("Assets/Ship.png", TextureType::Sprite);
	mSpriteComponent->AddSprite(shipSprite);
	mSpriteComponent->AddSprite(AssetManager::LoadTexture("Assets/ShipThrust.png", TextureType::Sprite));
	
	mSpriteComponent->SetSprite(shipSprite);

	SetSize(glm::vec2(shipSprite->GetWidth(), shipSprite->GetHeight()));
}

void Ship::OnProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse)
{
}

void Ship::OnUpdate(float deltaTime)
{
}
