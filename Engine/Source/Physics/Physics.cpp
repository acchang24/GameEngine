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

void Physics::HandleAABB2DvsAABB2D(AABBComponent2D* a, AABBComponent2D* b)
{
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
		}
		else if (minOverlap == std::abs(bottomEdge))
		{
			offset.y += bottomEdge;
		}
		else if (minOverlap == std::abs(leftEdge))
		{
			offset.x += leftEdge;
		}
		else if (minOverlap == std::abs(rightEdge))
		{
			offset.x += rightEdge;
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
