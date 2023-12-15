#include "Entity.h"
#include <iostream>
#include "Component.h"

Entity::Entity()
{

}

Entity::~Entity()
{
	std::cout << "Delete entity" << std::endl;

	for (auto c : mComponents)
	{
		delete c;
	}
	mComponents.clear();
}

void Entity::Update(float deltaTime)
{
	// Update all components
	for (auto c : mComponents)
	{
		c->Update(deltaTime);
	}
}

void Entity::Draw()
{
}
