#pragma once
#include <vector>
#include "../Components/CollisionComponent.h"

struct CollisionResult
{
	CollisionSide sideA;
	CollisionSide sideB;
};

class Physics
{
public:
	Physics();
	~Physics();

	// Updates all physics colliders
	// @param - float delta time
	void Update(float deltaTime);

	// Adds a collision component to vector of colliders
	// @param - CollisionComponent* for the new collision
	void AddCollider(CollisionComponent* collider) { mColliders.emplace_back(collider); }

	// Removes a collision component from the vector of colliders
	// @param - CollisionComponent* for the collision component to remove
	void RemoveCollider(CollisionComponent* collider);

private:
	// Handles collision between 2 AABB2D collision components:
	// First checks to see if the two AABB2D boxes intersects,
	// then applies offset to the position depending on the body type.
	// It then returns the collision result that contains collision sides for two objects
	// @param - const AABBComponent2D* for the first 2D AABB
	// @param - const AABBComponent2D* for the second 2D AABB
	// @return - CollisionResult for sides that got collided
	CollisionResult HandleAABB2DvsAABB2D(AABBComponent2D* a, AABBComponent2D* b);

	// Checks intersection between two 2D AABB
	// @param - const AABBComponent2D* for the first 2D AABB
	// @param - const AABBComponent2D* for the second 2D AABB
	bool IntersectAABB2DvsAABB2D(const AABBComponent2D* a, const AABBComponent2D* b) const;

	// Array of collision component colliders
	std::vector<CollisionComponent*> mColliders;
};
