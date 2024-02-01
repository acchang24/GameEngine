#pragma once
#include "Component.h"
#include "../Animation/Skeleton.h"
#include "../Graphics/UniformBuffer.h"

class Entity;
class Animation;
class Skeleton;

// Component to help animate 3D objects
class AnimationComponent : public Component
{
public:
	AnimationComponent(Entity* entity, Skeleton* skeleton);
	~AnimationComponent();

	// Override update for animation component specific updates
	void Update(float deltaTime) override;

	// Gets the skeleton's current animation
	// @return - Animation* for the skeleton's current animation
	Animation* GetCurrentAnimation() { return mSkeleton->GetCurrentAnimation(); }

	// Sets the animation component's skeleton
	// @param - Skeleton* for the new skeleton
	void SetSkeleton(Skeleton* s) { mSkeleton = s; }

	// Sets the skeleton's animation
	// @param - Animation* for the new animation for the skeleton to use
	void SetAnimation(Animation* a) { mSkeleton->SetAnimation(a); }

	// Adds an animation to the skelton's map of animations if it doesn't exist
	// @param - Animation* for the new animation
	void AddAnimation(Animation* a) { mSkeleton->AddAnimation(a); }

	// Updates the skeleton buffer with the final bone matrices array
	void UpdateBoneMatrices() { mSkeleton->GetSkeletonBuffer()->UpdateBufferData(mSkeleton->GetFinalBoneMatrices()); }

private:
	// The skeleton used for this animation component
	Skeleton* mSkeleton;
};
