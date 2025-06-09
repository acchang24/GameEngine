#pragma once
#include "Component.h"

class Entity2D;

class MoveComponent2D : public Component
{
public:
	MoveComponent2D(Entity2D* owner);
	~MoveComponent2D();

	// Override of update
	void Update(float deltaTime) override;

	// Gets the rotation/angular speed
	// @return - float for the speed of rotation
	float GetRotationSpeed() const { return mRotationSpeed; }

	// Gets the movement speed
	// @return - float for the speed of moving
	float GetMoveSpeed() const { return mMoveSpeed; }

	// Sets the rotation speed
	// @param - float for new speed
	void SetRotationSpeed(float speed) { mRotationSpeed = speed; }

	// Sets the movement speed
	// @param - float for new speed
	void SetMovementSpeed(float speed) { mMoveSpeed = speed; }

private:
	// Pointer to the Entity2D owner
	Entity2D* mOwner2D;

	// Rotation speed
	float mRotationSpeed;

	// Speed for moving in a direction
	float mMoveSpeed;
};
