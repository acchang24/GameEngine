#include "Ship.h"
#include <SDL2/SDL.h>
#include "Components/SpriteComponent.h"
#include "Graphics/Renderer2D.h"

Ship::Ship(Renderer2D* renderer) :
	Entity2D(renderer),
	mShipSprite(new SpriteComponent(this, renderer, "Assets/Ship.png"))
{
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	mShipSprite->SetTexture(mRenderer->LoadSpriteTexture("Assets/Ship.png"));

	if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
	{
		mShipSprite->SetTexture(mRenderer->LoadSpriteTexture("Assets/ShipThrust.png"));
	}
}

void Ship::OnUpdate(float deltaTime)
{
}
