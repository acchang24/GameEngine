#include "Component.h"
#include <iostream>
#include "../Entity/Entity.h"
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

void Component::ProcessInput(const InputSystem* input, const EngineContext& engineContext)
{

}

void Component::Update(float deltaTime, const EngineContext& engineContext)
{

}
