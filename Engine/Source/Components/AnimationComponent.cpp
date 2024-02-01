#include "AnimationComponent.h"
#include <iostream>
#include "../Graphics/UniformBuffer.h"
#include "../Profiler/Profiler.h"

AnimationComponent::AnimationComponent(Entity* entity, Skeleton* skeleton) :
	Component(entity),
	mSkeleton(skeleton)
{
}

AnimationComponent::~AnimationComponent()
{
	std::cout << "Delete animation component" << std::endl;
	delete mSkeleton;
}

void AnimationComponent::Update(float deltaTime)
{
	PROFILE_SCOPE(ANIMATE);

	mSkeleton->UpdateAnimation(deltaTime);
}
