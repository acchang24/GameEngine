#include "AnimationComponent.h"
#include <iostream>
#include "../Profiler/Profiler.h"

AnimationComponent::AnimationComponent(Entity* entity, Skeleton* skeleton) :
	Component(entity),
	mSkeleton(skeleton)
{
}

AnimationComponent::~AnimationComponent()
{
	delete mSkeleton;

	std::cout << "Deleted animation component at " << this << "\n";
}

void AnimationComponent::Update(float deltaTime)
{
	PROFILE_SCOPE(ANIMATE);

	mSkeleton->UpdateAnimation(deltaTime);
}
