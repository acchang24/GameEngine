#include "CollisionComponent.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "../Physics/Physics.h"

CollisionComponent::CollisionComponent(Entity* owner, Physics* physics, CollisionShapeType shapeType, BodyType bodyType) :
	Component(owner),
	mPhysics(physics),
	mShapeType(shapeType),
	mBodyType(bodyType)
{
	mPhysics->AddCollider(this);
}

CollisionComponent::~CollisionComponent()
{
	std::cout << "Deleted CollisionComponent\n";

	mPhysics->RemoveCollider(this);
}

void CollisionComponent::Update(float deltaTime)
{
}

AABBComponent2D::AABBComponent2D(Entity2D* owner, Physics* physics, BodyType bodyType) :
	CollisionComponent(owner, physics, CollisionShapeType::AABB2D, bodyType),
	mOwner2D(owner),
	mBox()
{
}

AABBComponent2D::~AABBComponent2D()
{
	std::cout << "Deleted AABBComponent2D\n";
}

void AABBComponent2D::Update(float deltaTime)
{
	// Update the box location
	mBox.min = GetMin();
	mBox.max = GetMax();
}


const glm::vec2 AABBComponent2D::GetMin() const
{
	glm::vec2 min = mOwner2D->GetPosition();
	float scale = mOwner2D->GetScale();

	// Minimum x value
	min.x -= (mBox.width * scale * 0.5f);
	// Minimum y value
	min.y -= (mBox.height * scale * 0.5f);

	return min;
}

const glm::vec2 AABBComponent2D::GetMax() const
{
	glm::vec2 max = mOwner2D->GetPosition();
	float scale = mOwner2D->GetScale();

	// Maximum x value
	max.x += (mBox.width * scale * 0.5f);
	// Maximum y value
	max.y += (mBox.height * scale * 0.5f);

	return max;
}


CircleComponent::CircleComponent(Entity2D* owner, Physics* physics, float radius, BodyType type) :
	CollisionComponent(owner, physics, CollisionShapeType::Circle, type),
	mOwner2D(owner),
	mCircle(radius)
{
}

CircleComponent::~CircleComponent()
{
	std::cout << "Deleted CircleComponent\n";
}
