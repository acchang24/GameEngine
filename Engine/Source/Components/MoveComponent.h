#pragma once
#include "Component.h"

class Entity;

class MoveComponent : public Component
{
public:
	MoveComponent(Entity* owner);
	~MoveComponent();

	// Override of Component class Update
	void Update(float deltaTime) override;

	// Gets the angular speed
	// @return - float for the angular speed
	float GetAngularSpeed() const { return mAngularSpeed; }

	// Gets the forward speed
	// @return - float for the forward speed
	float GetForwardSpeed() const { return mForwardSpeed; }

	// Sets the angular speed
	// @param - float for the new angular speed
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }

	// Sets the forward speed
	// @param - float for the new forward speed
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

protected:
	// Angular speed in radians/second
	float mAngularSpeed;

	// Forward speed in pixels/second
	float mForwardSpeed;
};
