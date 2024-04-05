#pragma once
#include "Component.h"
#include "../Animation/Skeleton.h"

class Entity;
class Animation;

// Component to used by 3D entities that animate
class AnimationComponent : public Component
{
public:
	// AnimationComponent constructor
	// @param - Entity* for the component's owner
	// @param - Skeleton* for the skeleton to animate
	AnimationComponent(Entity* entity, Skeleton* skeleton);
	~AnimationComponent();

	// Override update for animation component specific updates
	// Updates the skeleton's animation
	void Update(float deltaTime) override;

	// Gets the skeleton's current animation
	// @return - Animation* for the skeleton's current animation
	Animation* GetCurrentAnimation() { return mSkeleton->GetCurrentAnimation(); }

	// Sets the animation component's skeleton
	// @param - Skeleton* for the new skeleton
	void SetSkeleton(Skeleton* s) { mSkeleton = s; }

	// Updates the skeleton buffer with the final bone matrices array
	void UpdateSkeletonBuffer() { mSkeleton->UpdateSkeletonBuffer(); }

private:
	// The skeleton used for this animation component
	Skeleton* mSkeleton;
};
