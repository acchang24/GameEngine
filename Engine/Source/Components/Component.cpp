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
	std::cout << "Deleted component at " << this << "\n";
}

void Component::Update(float deltaTime)
{

}
