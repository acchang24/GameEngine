#include "CollisionComponent.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

CollisionComponent::CollisionComponent(Entity* owner, CollisionShapeType shapeType, BodyType bodyType) :
	Component(owner),
	mShapeType(shapeType),
	mBodyType(bodyType)
{
}

CollisionComponent::~CollisionComponent()
{
	std::cout << "Deleted CollisionComponent\n";
}

void CollisionComponent::Update(float deltaTime)
{
}

bool CollisionComponent::Intersects(const CollisionComponent* other) const
{
	return false;
}


AABBComponent2D::AABBComponent2D(Entity2D* owner, BodyType bodyType) :
	CollisionComponent(owner, CollisionShapeType::AABB2D, bodyType),
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

bool AABBComponent2D::Intersects(const CollisionComponent* other) const
{
	return false;
}

bool AABBComponent2D::Intersect(const AABBComponent2D* other) const
{
	bool case1 = other->mBox.max.x < mBox.min.x;
	bool case2 = mBox.max.x < other->mBox.min.x;
	bool case3 = other->mBox.max.y < mBox.min.y;
	bool case4 = mBox.max.y < other->mBox.min.y;

	return !(case1 || case2 || case3 || case4);
}

CollisionSide AABBComponent2D::GetMinOverlap(const AABBComponent2D* other, glm::vec2& offset)
{
	if (Intersect(other))
	{
		glm::vec2 position = mOwner2D->GetPosition();

		// Get the difference between two intersected boxes for each edge of the box
		float topEdge = (other->mBox.min.y - mBox.max.y);
		float bottomEdge = (other->mBox.max.y - mBox.min.y);
		float leftEdge = (other->mBox.min.x - mBox.max.x);
		float rightEdge = (other->mBox.max.x - mBox.min.x);

		// Find lowest absolute value for the closest edge
		float min = std::min({ std::abs(topEdge), std::abs(bottomEdge), std::abs(leftEdge), std::abs(rightEdge) });

		if (min == std::abs(topEdge))
		{
			offset.y += topEdge;
			return CollisionSide::Top;
		}
		else if (min == std::abs(bottomEdge))
		{
			offset.y += bottomEdge;
			return CollisionSide::Bottom;
		}
		else if (min == std::abs(leftEdge))
		{
			offset.x += leftEdge;
			return CollisionSide::Left;
		}
		else if (min == std::abs(rightEdge))
		{
			offset.x += rightEdge;
			return CollisionSide::Right;
		}
	}

	return CollisionSide::None;
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


