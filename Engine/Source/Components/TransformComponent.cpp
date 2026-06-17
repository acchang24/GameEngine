#include "TransformComponent.h"
#include <iostream>

TransformComponent::TransformComponent(Entity* owner) : 
	Component(owner)
{

}

TransformComponent::~TransformComponent()
{
	std::cout << "Deleted Transform Component\n";
}
