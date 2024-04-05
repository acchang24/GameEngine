#include "Entity.h"
#include <iostream>
#include "../Components/Component.h"

Entity::Entity()
{

}

Entity::~Entity()
{
	for (auto c : mComponents)
	{
		delete c;
	}
	mComponents.clear();

	std::cout << "Delete entity" << std::endl;
}

void Entity::Update(float deltaTime)
{
	for (auto c : mComponents)
	{
		c->Update(deltaTime);
	}
}

void Entity::Draw()
{
}
