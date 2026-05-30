#include "Component.h"
#include <iostream>
#include "../Entity/Entity.h"
#include "../Input/Mouse.h"
#include "../Input/InputSystem.h"

Component::Component(Entity* owner) : 
	mOwner(owner)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	std::cout << "Deleted component\n";
}

void Component::ProcessInput(const InputSystem* input)
{

}

void Component::Update(float deltaTime)
{

}
