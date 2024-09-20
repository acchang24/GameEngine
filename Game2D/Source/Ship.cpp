#include "Ship.h"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <SDL2/SDL.h>
#include "Components/MoveComponent.h"
#include "Components/SpriteComponent.h"
#include "Graphics/Renderer2D.h"

Ship::Ship(Renderer2D* renderer) :
	Entity2D(renderer),
	mMove(new MoveComponent(this)),
	mShipSprite(new SpriteComponent(this, renderer, "Assets/Ship.png"))
{
	mRotation = glm::half_pi<float>();
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	float angularSpeed = 0.0f;
	float speed = 0.0f;

	mShipSprite->SetTexture(mRenderer->LoadSpriteTexture("Assets/Ship.png"));

	if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W])
	{
		speed += 100.0f;
	}
	if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S])
	{
		speed -= 100.0f;
	}
	if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
	{
		angularSpeed += 4.0f;
	}
	if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
	{
		angularSpeed -= 4.0f;
	}

	if (speed != 0.0f)
	{
		mShipSprite->SetTexture(mRenderer->LoadSpriteTexture("Assets/ShipThrust.png"));
	}

	glm::vec2 p = GetPosition();
	glm::vec2 f = GetForward();
	std::cout << p.x << " " << p.y << " " << f.x << " " << f.y << "\n";

	mMove->SetAngularSpeed(angularSpeed);
	mMove->SetForwardSpeed(speed);
}

void Ship::OnUpdate(float deltaTime)
{
}
