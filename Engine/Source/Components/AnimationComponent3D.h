#pragma once
#include "Component.h"
#include <string>
#include <unordered_map>
#include "../Animation/BoneData.h"
#include "../Multithreading/JobManager.h"
#include "../EngineContext.h"

class Animation;
class Entity;
class Skeleton;
class UniformBuffer;
class Model;


// Component to used by 3D entities that animate
class AnimationComponent3D : public Component
{
public:
	// AnimationComponent constructor: 
	// Checks to see if the skelton exists, and loads all the saved animations into the map of animations. Sets default animation at the end
	// @param - Entity* for the component's owner
	// @param - Model* for the 
	AnimationComponent3D(Entity* entity, Skeleton* skeleton, UniformBuffer* skeletonBuffer);

	// AnimationComponent destructor:
	// mSkeletonBuffer, mSkeleton, and mCurrentAnimation are all loaded through AssetManager, with mSkeletonBuffer being
	// loaded through the Renderer3D. These are all cached/shared objects so do not free/call delete on these here.
	~AnimationComponent3D();

	// Override update for animation component specific updates
	// Updates the animation time and gets the skeletons final pose at that time
	// @param - float for delta time
	// @param - const EngineContext& for the engine context
	void Update(float deltaTime, const EngineContext& engineContext) override;

	// Updates the skeleton buffer with the final bone matrices array
	void UpdateSkeletonBuffer();

	// Gets the skeleton's current animation
	// @return - Animation* for the skeleton's current animation
	Animation* GetCurrentAnimation() { return mCurrentAnimation; }

	// Sets the current animation by animation if it exists, and resets timer
	// @param - Animation* for the new animation
	void SetCurrentAnimation(Animation* anim);

	// Sets the current animation by animation name if it exists and resets timer.
	// Remains unchanged if animation doesn't exist
	// @param - const std::string& for the name
	void SetCurrentAnimation(const std::string& name);

	// Sets the current time
	// @param - float for the new time
	void SetTime(float time) { mCurrentTime = time; }

private:
	// Job to update bone matrices on a separate threads
	class UpdateBoneJob : public JobManager::Job
	{
	public:
		UpdateBoneJob(AnimationComponent3D* comp) : mComp(comp)
		{
		}
		void DoJob() override;
	private:
		AnimationComponent3D* mComp;
	};

	// Sets the default animation (first animation in the map)
	void SetDefaultAnim();

	// Loads the cached animations from the skeleton and save them into the animations map
	// @param - const Skleleton* for the cached skeleton
	void LoadCachedAnims(const Skeleton* skeleton);



	// Map of animations by name
	std::unordered_map<std::string, Animation*> mAnimations;

	// Skeleton constants for final bone matrix palette
	SkeletonConsts mSkeletonConsts;

	// The skeleton used for this animation component
	Skeleton* mSkeleton;

	// The current animation
	Animation* mCurrentAnimation;

	// Uniform buffer to send skelton data to
	UniformBuffer* mSkeletonBuffer;

	// Job to update bone transformations on separate thread
	UpdateBoneJob mJob;

	// Current time of the animation
	float mCurrentTime;
};
