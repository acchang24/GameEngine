#pragma once
#include <vector>
#include "../Components/CollisionComponent.h"


class Physics
{
public:
	Physics();
	~Physics();

	// Adds a collision component to vector of colliders
	// @param - CollisionComponent* for the new collision
	void AddCollider(CollisionComponent* collider) { mColliders.emplace_back(collider); }

	// Removes a collision component from the vector of colliders
	// @param - CollisionComponent* for the collision component to remove
	void RemoveCollider(CollisionComponent* collider);

private:

	std::vector<CollisionComponent*> mColliders;

};
