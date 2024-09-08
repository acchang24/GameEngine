#include "MoveComponent.h"
#include <iostream>
#include "../Entity/Entity.h"
#include "../Entity/Entity2D.h"
#include "../Entity/Entity3D.h"

MoveComponent::MoveComponent(Entity* owner) :
	Component(owner),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f)
{
}

MoveComponent::~MoveComponent()
{
	std::cout << "Delete MoveComponent\n";
}

void MoveComponent::Update(float deltaTime)
{
	// Update entity's rotation by the angular speed
	mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed * deltaTime);

	if (dynamic_cast<Entity3D*>(mOwner))
	{
		std::cout << "3d\n";
	}
	else if (dynamic_cast<Entity2D*>(mOwner))
	{
		Entity2D* owner = static_cast<Entity2D*>(mOwner);
		glm::vec2 move = owner->GetForward() * mForwardSpeed * deltaTime;
		owner->SetPosition(owner->GetPosition() + move);
	}
}
