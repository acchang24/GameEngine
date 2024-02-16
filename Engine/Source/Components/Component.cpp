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
	std::cout << "Delete component\n";
}

void Component::Update(float deltaTime)
{

}
