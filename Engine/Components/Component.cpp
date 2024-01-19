#include "Component.h"
#include <iostream>
#include "../Entity.h"

Component::Component(Entity* owner) : 
	mOwner(owner)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	std::cout << "Delete component" << std::endl;
}

void Component::Update(float deltaTime)
{

}
