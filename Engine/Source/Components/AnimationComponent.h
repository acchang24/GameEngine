#pragma once
#include "Component.h"
#include <string>
#include <unordered_map>
#include <assimp/scene.h>
#include "../Animation/BoneData.h"
#include "../Multithreading/JobManager.h"

class Animation;
class Entity;
class Skeleton;
class UniformBuffer;

// Component to used by 3D entities that animate
class AnimationComponent : public Component
{
public:
	// AnimationComponent constructor: Checks the AssetManager to see if a skeleton was created by the model's file
	// name. If it was loaded, just use the cached skeleton and animatins. Creates a skeleton if it hasn't been loaded, 
	// and loads and saves all the animations in the animation map.
	// @param - Entity* for the component's owner
	// @param - const aiScene* for the model's scene
	// @param - const std::string& for the file name to check if it's been loaded already
	AnimationComponent(Entity* entity, const aiScene* scene, const std::string& fileName);
	// Overload AnimationComponent constructor:
	// @param - Entity* for the component's owner
	// @param - Skeleton* for the cached skeleton to copy data
	AnimationComponent(Entity* entity, Skeleton* skeleton);
	// AnimationComponent destructor:
	// mSkeletonBuffer, mSkeleton, and mCurrentAnimation are all loaded through AssetManager, with mSkeletonBuffer being
	// loaded through the Renderer3D. These are all cached/shared objects so do not free/call delete on these here.
	~AnimationComponent();

	// Loops through the model's scene's animations and creates an Animation* for each one,
	// saving them into the map of Animations* by name
	// @param - const aiScene* for the model's scene
	void LoadAnimations(const aiScene* scene);

	// Override update for animation component specific updates
	// Updates the animation time and gets the skeletons final pose at that time
	void Update(float deltaTime) override;

	// Updates the skeleton buffer with the final bone matrices array
	void UpdateSkeletonBuffer();

	// Gets the skeleton associated with this component
	// @return - Skeleton* for the skeleton
	Skeleton* GetSkeleton() { return mSkeleton; }

	// Gets the skeleton's current animation
	// @return - Animation* for the skeleton's current animation
	Animation* GetCurrentAnimation() { return mCurrentAnimation; }

	// Sets the animation component's skeleton
	// @param - Skeleton* for the new skeleton
	void SetSkeleton(Skeleton* s) { mSkeleton = s; }

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
		UpdateBoneJob(AnimationComponent* comp) : mComp(comp)
		{
		}
		void DoJob() override;
	private:
		AnimationComponent* mComp;
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

	UpdateBoneJob mJob;

	// Current time of the animation
	float mCurrentTime;
};
