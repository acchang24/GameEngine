#pragma once
#include <vector>
#include "../Components/CollisionComponent.h"

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

	// Checks intersection between two 2D AABB
	// @param - const AABBComponent2D* for the first 2D AABB
	// @param - const AABBComponent2D* for the second 2D AABB
	// @return - bool for if the two AABBs intersect
	static bool IntersectAABB2DvsAABB2D(const AABBComponent2D* a, const AABBComponent2D* b);

	// Checks intersection between two circles and updates the offset vector
	// @param - const CircleComponent* for the first circle
	// @param - const CircleComponent* for the second circle
	// @param - glm::vec2& for the offset vector
	// @return - bool for if the two circles intersect
	static bool IntersectCircleVsCircle(const CircleComponent* a, const CircleComponent* b, glm::vec2& offset);

	// Checks intersection between two 2D OBB
	// @param - const OBBComponent2D* for the first 2D OBB
	// @param - const OBBComponent2D* for the second 2D OBB
	// @param - glm::vec2& for the offset vector
	// @return - bool for if the two AABBs intersect
	static bool IntersectOBB2DvsOBB2D(const OBBComponent2D* a, const OBBComponent2D* b, glm::vec2& offset);

	// Checks intersection between a Circle and an AABB2D
	// @param - const CircleComponent* for the circle
	// @param - const AABBComponent2D* for the 2D AABB
	// @param - glm::vec2& for the offset vector
	// @return - bool for if the circle and AABB intersect
	static bool IntersectCircleVsAABB2D(const CircleComponent* circle, const AABBComponent2D* aabb, glm::vec2& offset);

	// Checks intersection between 2D OBB and 2D AABB
	// @param - const OBBComponent2D* for the obb
	// @param - const AABBComponent2D* for the aabb
	// @param - glm::vec2& for the offset vector
	// @return - bool for if the OBB and AABB intersect
	static bool IntersectOBB2DvsAABB2D(const OBBComponent2D* obb, const AABBComponent2D* aabb, glm::vec2& offset);

	// Projects corners onto an axis
	static void ProjectOnAxis(const std::array<glm::vec2, 4>& corners, const glm::vec2& axis, float& min, float& max);

private:
	// Handles collision between 2 AABB2D collision components:
	// First checks to see if the two AABB2D boxes intersects,
	// then applies offset to the position depending on the body type.
	// It then returns the collision result that contains collision sides for two objects
	// @param - AABBComponent2D* for the first 2D AABB
	// @param - AABBComponent2D* for the second 2D AABB
	// @return - CollisionResult for sides that got collided
	CollisionResult HandleAABB2DvsAABB2D(AABBComponent2D* a, AABBComponent2D* b);

	// Handles collision between 2 circle collision components:
	// First checks to see if the circles intersect,
	// then applies offset to position depending on the body type
	// It then returns the collision result that contains collision sides for two objects
	// @param - CircleComponent* for the first circle
	// @param - CircleComponent* for the second circle
	// @return - CollisionResult for the sides that got collided
	CollisionResult HandleCircleVsCircle(CircleComponent* a, CircleComponent* b);

	// Handles collision between 2 OBB collision components:
	// First checks to see if the two OBB boxes intersects,
	// then applies offset to the position depending on the body type.
	// It then returns the collision result that contains collision sides for two objects
	// @param - OBBComponent2D* for the first 2D OBB
	// @param - OBBComponent2D* for the second 2D OBB
	// @return - CollisionResult for sides that got collided
	CollisionResult HandleOBB2DvsOBB2D(OBBComponent2D* a, OBBComponent2D* b);

	// Handles collision between circle and AABB2D collision components:
	// First checks to see if the circle and AABB intersect,
	// then applies offset to position depending on the body type
	// It then returns the collision result that contains collision sides for two objects
	// @param - CircleComponent* for the circle
	// @param - AABBComponent2D* for the 2d AABB
	// @return - CollisionResult for the sides that got collided
	CollisionResult HandleCircleVsAABB2D(CircleComponent* circle, AABBComponent2D* aabb);

	// Handles collision between OBB2D and AABB2D collision components:
	// First checks to see if the OBB2D and AABB intersect,
	// then applies offset to position depending on the body type
	// It then returns the collision result that contains collision sides for two objects
	// @param - OBBComponent2D* for the 2d OBB
	// @param - AABBComponent2D* for the 2d AABB
	// @return - CollisionResult for the sides that got collided
	CollisionResult HandleOBB2DVsAABB2D(OBBComponent2D* obb, AABBComponent2D* aabb);

	// Applies offset to 2D entities based on the body type
	// @param - Entity2D* for the first entity
	// @param - Entity2D* for the second entity
	// @param - BodyType for the first body type
	// @param - BodyType for the second body type
	// @param - const glm::vec2& for the offset to apply
	void ApplyOffset2D(Entity2D* a, Entity2D* b, BodyType bodyA, BodyType bodyB, const glm::vec2& offset);

	// Array of collision component colliders
	std::vector<CollisionComponent*> mColliders;
};
