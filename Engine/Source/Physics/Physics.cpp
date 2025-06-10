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

				HandleAABB2DvsAABB2D(collisionA, collisionB);
			}
			// Handle 2 Circle collision
			if (a->GetShapeType() == CollisionShapeType::Circle && b->GetShapeType() == CollisionShapeType::Circle)
			{
				// Cast to circle component
				CircleComponent* collisionA = static_cast<CircleComponent*>(a);
				CircleComponent* collisionB = static_cast<CircleComponent*>(b);

				HandleCircleVsCircle(collisionA, collisionB);
			}

			// Circle vs AABB (both orders)
			if ((a->GetShapeType() == CollisionShapeType::Circle && b->GetShapeType() == CollisionShapeType::AABB2D) ||
				(a->GetShapeType() == CollisionShapeType::AABB2D && b->GetShapeType() == CollisionShapeType::Circle))
			{
				CircleComponent* circle = nullptr;
				AABBComponent2D* box = nullptr;
				if(a->GetShapeType() == CollisionShapeType::Circle)
				{
					circle = static_cast<CircleComponent*>(a);
					box = static_cast<AABBComponent2D*>(b);
				}
				else
				{
					circle = static_cast<CircleComponent*>(b);
					box = static_cast<AABBComponent2D*>(a);
				}

				HandleCircleVsAABB2D(circle, box);
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

bool Physics::IntersectAABB2DvsAABB2D(const AABBComponent2D* a, const AABBComponent2D* b)
{
	const AABB_2D& boxA = a->GetBox();
	const AABB_2D& boxB = b->GetBox();

	bool case1 = boxB.max.x < boxA.min.x;
	bool case2 = boxA.max.x < boxB.min.x;
	bool case3 = boxB.max.y < boxA.min.y;
	bool case4 = boxA.max.y < boxB.min.y;

	return !(case1 || case2 || case3 || case4);
}

bool Physics::IntersectCircleVsCircle(const CircleComponent* a, const CircleComponent* b, glm::vec2& offset)
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

bool Physics::IntersectCircleVsAABB2D(const CircleComponent* circle, const AABBComponent2D* aabb, glm::vec2& offset)
{
	const glm::vec2 circleCenter = circle->GetCenter();
	const AABB_2D& box = aabb->GetBox();

	// Clamp the circle to the AABB bounds to get closest point on/inside the AABB to circle center
	glm::vec2 clamped = glm::clamp(circleCenter, box.min, box.max);

	// Get the vector from clamped circle center to the circle center
	glm::vec2 v = circleCenter - clamped;

	// get the distance from that vector
	float distance = glm::length(v);

	// Rare case distance is 0.0, set arbitrary push direction
	if (distance == 0.0f)
	{
		offset = glm::vec2(1.0f, 0.0f);
		distance = 1.0f;
	}

	// Direction from that vector
	glm::vec2 direction = glm::normalize(v);

	float overlap = circle->GetCircle().radius - distance;
	offset = direction * overlap;

	// Use squared distance to compare with radius squared
	float distanceSq = glm::dot(v, v);
	float radius = circle->GetCircle().radius;

	return distanceSq < radius * radius;
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

		BodyType typeA = a->GetBodyType();
		BodyType typeB = b->GetBodyType();

		ApplyOffset2D(ownerA, ownerB, typeA, typeB, offset);

		a->OnCollision(b->GetOwner(), result);
		b->OnCollision(a->GetOwner(), result);
	}

	return result;
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

		ApplyOffset2D(ownerA, ownerB, typeA, typeB, offset);

		// No result update, circle's don't have sides I guess?

		a->OnCollision(b->GetOwner(), result);
		b->OnCollision(a->GetOwner(), result);

	}

	return result;
}

CollisionResult Physics::HandleCircleVsAABB2D(CircleComponent* circle, AABBComponent2D* aabb)
{
	CollisionResult result = { CollisionSide::None, CollisionSide::None };

	// Offset
	glm::vec2 offset(0.0f);

	if (IntersectCircleVsAABB2D(circle, aabb, offset))
	{
		Entity2D* ownerCircle = circle->GetOwner();
		Entity2D* ownerAABB = aabb->GetOwner();
		BodyType typeCircle = circle->GetBodyType();
		BodyType typeAABB = aabb->GetBodyType();

		ApplyOffset2D(ownerCircle, ownerAABB, typeCircle, typeAABB, offset);

		// Get side for AABB
		glm::vec2 diff = circle->GetCenter() - aabb->GetCenter();

		if (std::abs(diff.x) > std::abs(diff.y))
		{
			// More horizontal overlap
			if (diff.x > 0)
			{
				result.sideB = CollisionSide::Right;
			}
			else
			{
				result.sideB = CollisionSide::Left;
			}
		}
		else
		{
			// More vertical overlap
			if (diff.y > 0) 
			{
				result.sideB = CollisionSide::Top;
			} 
			else
			{
				result.sideB = CollisionSide::Bottom;
			}
		}

		circle->OnCollision(aabb->GetOwner(), result);
		aabb->OnCollision(circle->GetOwner(), result);
	}

	return result;
}

void Physics::ApplyOffset2D(Entity2D* a, Entity2D* b, BodyType bodyA, BodyType bodyB, const glm::vec2& offset)
{
	// Set the offset based on body type
	if (bodyA == BodyType::Dynamic && bodyB == BodyType::Static)
	{
		// Apply offset to owner (a) that initiated collision
		a->SetPosition(a->GetPosition() + offset);
	}
	else if (bodyA == BodyType::Static && bodyB == BodyType::Dynamic)
	{
		// Apply offset to owner (b) that initiated collision
		b->SetPosition(b->GetPosition() - offset);
	}
	else if (bodyA == BodyType::Dynamic && bodyB == BodyType::Dynamic)
	{
		// Split offset to both if they are both dynamic
		a->SetPosition(a->GetPosition() + offset * 0.5f);
		b->SetPosition(b->GetPosition() - offset * 0.5f);
	}
}
