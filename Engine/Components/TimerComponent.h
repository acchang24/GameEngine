#pragma once
#include "Component.h"

class Entity;

// Component to set a timer
class TimerComponent : public Component
{
public:
	TimerComponent(Entity* owner);
	~TimerComponent();

	// Increments the mTimeElapsed variable with delta time
	void Update(float deltaTime) override;

	float GetTimeElapsed() const { return mTimeElapsed; }

private:
	float mTimeElapsed;
};