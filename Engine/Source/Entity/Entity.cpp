#include "Entity.h"
#include <iostream>
#include "../Components/Component.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

Entity::Entity() :
	mState(EntityState::Active),
	mRotation(0.0f),
	mPos2D(glm::vec2(0.0f, 0.0f)),
	mSize(glm::vec2(5.0f, 5.0f)),
	mFloatScale(1.0f)
{

}

Entity::~Entity()
{
	std::cout << "Deleted entity\n";

	for (auto c : mComponents)
	{
		delete c;
	}
	mComponents.clear();
}

void Entity::ProcessInput(const Uint8* keyState, const Keyboard* keyboard, const Mouse* mouse)
{
	if (mState == EntityState::Active)
	{
		for (auto c : mComponents)
		{
			c->ProcessInput(keyState, mouse);
		}
	}
}

void Entity::Update(float deltaTime)
{
	if (mState == EntityState::Active)
	{
		for (auto c : mComponents)
		{
			c->Update(deltaTime);
		}
	}
}

void Entity::Draw()
{

}
