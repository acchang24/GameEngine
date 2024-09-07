#include "Component.h"
#include <iostream>
#include "../Entity/Entity.h"

Component::Component(Entity* owner) : 
	mOwner(owner)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	std::cout << "Deleted component\n";
}

void Component::ProcessInput(const Uint8* keyState)
{

}

void Component::Update(float deltaTime)
{

}
