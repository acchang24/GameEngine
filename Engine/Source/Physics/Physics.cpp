#include "Physics.h"
#include <algorithm>
#include <iostream>

Physics::Physics()
{
}

Physics::~Physics()
{
	std::cout << "Deleted Physics\n";
}

void Physics::Update(float deltaTime)
{
	for (size_t i = 0; i < mColliders.size(); ++i)
	{
		for (size_t j = i + 1; j < mColliders.size(); ++j)
		{
			CollisionComponent* a = mColliders[i];
			CollisionComponent* b = mColliders[j];
			
			// Handle 2 AABB2D collision
			if (a->GetShapeType() == CollisionShapeType::AABB2D && b->GetShapeType() == CollisionShapeType::AABB2D)
			{
				// Use static cast since we know the type for sure
				AABBComponent2D* collisionA = static_cast<AABBComponent2D*>(a);
				AABBComponent2D* collisionB = static_cast<AABBComponent2D*>(b);

				CollisionResult result = HandleAABB2DvsAABB2D(collisionA, collisionB);

			}
			// Handle 2 Circle collision
			if (a->GetShapeType() == CollisionShapeType::Circle && b->GetShapeType() == CollisionShapeType::Circle)
			{
				// Cast to circle component
				CircleComponent* collisionA = static_cast<CircleComponent*>(a);
				CircleComponent* collisionB = static_cast<CircleComponent*>(b);

				HandleCircleVsCircle(collisionA, collisionB);
			}
		}
	}
}

void Physics::RemoveCollider(CollisionComponent* collider)
{
	auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
	if (iter != mColliders.end())
	{
		mColliders.erase(iter);
	}
}

CollisionResult Physics::HandleAABB2DvsAABB2D(AABBComponent2D* a, AABBComponent2D* b)
{
	CollisionResult result = { CollisionSide::None, CollisionSide::None };

	if (IntersectAABB2DvsAABB2D(a, b))
	{
		// Offset position to push back objects
		glm::vec2 offset(0.0f);

		Entity2D* ownerA = a->GetOwner();
		Entity2D* ownerB = b->GetOwner();

		const AABB_2D& boxA = a->GetBox();
		const AABB_2D& boxB = b->GetBox();

		// Get the difference between two intersected boxes for each edge of the box
		float topEdge = (boxB.min.y - boxA.max.y);
		float bottomEdge = (boxB.max.y - boxA.min.y);
		float leftEdge = (boxB.min.x - boxA.max.x);
		float rightEdge = (boxB.max.x - boxA.min.x);

		// Find lowest absolute value for the closest edge
		float minOverlap = std::min({ std::abs(topEdge), std::abs(bottomEdge), std::abs(leftEdge), std::abs(rightEdge) });

		if (minOverlap == std::abs(topEdge))
		{
			offset.y += topEdge;
			result.sideA = CollisionSide::Bottom;
			result.sideB = CollisionSide::Top;
		}
		else if (minOverlap == std::abs(bottomEdge))
		{
			offset.y += bottomEdge;

			result.sideA = CollisionSide::Top;
			result.sideB = CollisionSide::Bottom;
		}
		else if (minOverlap == std::abs(leftEdge))
		{
			offset.x += leftEdge;
			result.sideA = CollisionSide::Right;
			result.sideB = CollisionSide::Left;
		}
		else if (minOverlap == std::abs(rightEdge))
		{
			offset.x += rightEdge;

			result.sideA = CollisionSide::Left;
			result.sideB = CollisionSide::Right;
		}

		// Apply offset based on body type
		BodyType typeA = a->GetBodyType();
		BodyType typeB = b->GetBodyType();

		if (typeA == BodyType::Dynamic && typeB == BodyType::Static)
		{
			// Apply offset to owner (a) that initiated collision
			ownerA->SetPosition(ownerA->GetPosition() + offset);
		}
		else if (typeA == BodyType::Static && typeB == BodyType::Dynamic)
		{
			// Apply offset to owner (b) that initiated collision
			ownerB->SetPosition(ownerB->GetPosition() - offset);
		}
		else if (typeA == BodyType::Dynamic && typeB == BodyType::Dynamic)
		{
			// Split offset to both if they are both dynamic
			ownerA->SetPosition(ownerA->GetPosition() + offset * 0.5f);
			ownerB->SetPosition(ownerB->GetPosition() - offset * 0.5f);
		}
	}

	return result;
}

bool Physics::IntersectAABB2DvsAABB2D(const AABBComponent2D* a, const AABBComponent2D* b) const
{
	const AABB_2D& boxA = a->GetBox();
	const AABB_2D& boxB = b->GetBox();

	bool case1 = boxB.max.x < boxA.min.x;
	bool case2 = boxA.max.x < boxB.min.x;
	bool case3 = boxB.max.y < boxA.min.y;
	bool case4 = boxA.max.y < boxB.min.y;

	return !(case1 || case2 || case3 || case4);
}

CollisionResult Physics::HandleCircleVsCircle(CircleComponent* a, CircleComponent* b)
{
	CollisionResult result = { CollisionSide::None, CollisionSide::None };

	glm::vec2 offset(0.0f);

	if (IntersectCircleVsCircle(a, b, offset))
	{
		Entity2D* ownerA = a->GetOwner();
		Entity2D* ownerB = b->GetOwner();

		const Circle2D& circleA = a->GetCircle();
		const Circle2D& circleB = b->GetCircle();

		BodyType typeA = a->GetBodyType();
		BodyType typeB = b->GetBodyType();

		// Set the offset based on body type
		if (typeA == BodyType::Dynamic && typeB == BodyType::Static)
		{
			// Apply offset to owner (a) that initiated collision
			ownerA->SetPosition(ownerA->GetPosition() + offset);
		}
		else if (typeA == BodyType::Static && typeB == BodyType::Dynamic)
		{
			// Apply offset to owner (b) that initiated collision
			ownerB->SetPosition(ownerB->GetPosition() - offset);
		}
		else if (typeA == BodyType::Dynamic && typeB == BodyType::Dynamic)
		{
			// Split offset to both if they are both dynamic
			ownerA->SetPosition(ownerA->GetPosition() + offset * 0.5f);
			ownerB->SetPosition(ownerB->GetPosition() - offset * 0.5f);
		}

		// No result update, circle's don't have sides I guess?
	}

	return result;
}

bool Physics::IntersectCircleVsCircle(const CircleComponent* a, const CircleComponent* b, glm::vec2& offset) const
{
	// Get the vector from b to a
	glm::vec2 v = a->GetCenter() - b->GetCenter();

	// Get the length of vector b to a
	float distance = glm::length(v);

	// Get the total radius between a and b
	float radiusSum = a->GetCircle().radius + b->GetCircle().radius;

	// if distance is less than radius sum then it intersects
	if (distance < radiusSum)
	{
		// Check if a and b are in same position (rare occasion)
		if (distance == 0.0f)
		{
			// Arbitrary direction
			offset = glm::vec2(radiusSum, 0.0f);
		}
		else
		{
			// Get the direction from b to a
			glm::vec2 direction = glm::normalize(v);

			float overlap = radiusSum - distance;

			offset = direction * overlap;
		}

		return true;
	}

	return false;
}
