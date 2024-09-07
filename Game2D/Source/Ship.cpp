#include "Ship.h"
#include "Components/SpriteComponent.h"

Ship::Ship(Renderer2D* renderer) :
	Entity2D(),
	mShipSprite(new SpriteComponent(this, renderer, "Assets/Ship.png"))
{
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	
}

void Ship::OnUpdate(float deltaTime)
{
}
