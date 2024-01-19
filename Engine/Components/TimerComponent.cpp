#include "TimerComponent.h"
#include <iostream>

TimerComponent::TimerComponent(Entity* owner) :
	Component(owner),
	mTimeElapsed(0.0f)
{

}

TimerComponent::~TimerComponent()
{
	std::cout << "Delete timer component" << std::endl;
}

void TimerComponent::Update(float deltaTime)
{
	mTimeElapsed += deltaTime;
}
