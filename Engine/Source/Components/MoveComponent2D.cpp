#include "MoveComponent2D.h"
#include <iostream>
#include "../Entity/Entity2D.h"
#include <glm/gtc/quaternion.hpp>

MoveComponent2D::MoveComponent2D(Entity2D* owner) :
	Component(owner),
	mOwner2D(owner),
	mRotationSpeed(0.0f),
	mMoveSpeed(0.0f)
{
}

MoveComponent2D::~MoveComponent2D()
{
	std::cout << "Deleted MoveComponent2D\n";
}

void MoveComponent2D::Update(float deltaTime, const EngineContext& engineContext)
{
	mOwner2D->SetRotation2D(mOwner2D->GetQuatRotation() * glm::angleAxis((mRotationSpeed * deltaTime) , glm::vec3(0.0f, 0.0f, 1.0f)));

	mOwner2D->SetPosition2D(mOwner2D->GetPosition2D() + mOwner2D->GetForward2D() * mMoveSpeed * deltaTime);
}

