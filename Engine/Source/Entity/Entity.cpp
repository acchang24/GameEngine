#include "Entity.h"
#include <iostream>
#include "../Components/Component.h"

Entity::Entity() :
	mState(EntityState::Active),
	mRotation(0.0f)
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

void Entity::ProcessInput(const Uint8* keyState)
{
	if (mState == EntityState::Active)
	{
		for (auto c : mComponents)
		{
			c->ProcessInput(keyState);
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
