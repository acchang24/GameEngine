#include "MoveComponent2D.h"
#include <iostream>
#include "../Entity/Entity2D.h"

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

void MoveComponent2D::Update(float deltaTime)
{
	mOwner2D->SetRotation(mOwner2D->GetRotation() + mRotationSpeed * deltaTime);

	mOwner2D->SetPosition(mOwner2D->GetPosition() + mOwner2D->GetForward() * mMoveSpeed * deltaTime);
}

