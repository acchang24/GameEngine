#include "AnimationComponent.h"
#include <iostream>
#include "../Animation/Skeleton.h"
#include "../Animation/Animation.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/UniformBuffer.h"
#include "../MemoryManager/AssetManager.h"

AnimationComponent::AnimationComponent(Entity* entity, const aiScene* scene, const std::string& fileName) :
	Component(entity),
	mSkeletonConsts({}),
	mSkeleton(nullptr),
	mCurrentAnimation(nullptr),
	mSkeletonBuffer(AssetManager::Get()->GetRenderer()->GetUniformBuffer("SkeletonBuffer")),
	mJob(this),
	mCurrentTime(0.0f)
{
	// Check to see if skeleton was already loaded
	mSkeleton = AssetManager::LoadSkeleton(fileName);

	if (!mSkeleton)
	{
		// Create a new skeleton
		mSkeleton = new Skeleton(scene, fileName);

		// Save skeleton into AssetManager
		AssetManager::SaveSkeleton(fileName, mSkeleton);

		LoadAnimations(scene, fileName);
	}
	else
	{
		LoadCachedAnims(mSkeleton);
	}

	// Set default animation to first anim in map
	SetDefaultAnim();

	// If there isn't skeleton buffer loaded within renderer
	if (!mSkeletonBuffer)
	{
		// Create a new skeleton buffer within renderer (this component will not have ownership)
		mSkeletonBuffer = AssetManager::Get()->GetRenderer()->CreateUniformBuffer(sizeof(SkeletonConsts), BufferBindingPoint::Skeleton, "SkeletonBuffer");
	}
}

AnimationComponent::AnimationComponent(Entity* entity, Skeleton* skeleton) :
	Component(entity),
	mSkeletonConsts({}),
	mSkeleton(skeleton),
	mCurrentAnimation(nullptr),
	mSkeletonBuffer(AssetManager::Get()->GetRenderer()->GetUniformBuffer("SkeletonBuffer")),
	mJob(this),
	mCurrentTime(0.0f)
{
	LoadCachedAnims(mSkeleton);

	// Set default animation to first anim in map
	SetDefaultAnim();
}

AnimationComponent::~AnimationComponent()
{
	std::cout << "Deleted animation component\n";
}

void AnimationComponent::LoadAnimations(const aiScene* scene, const std::string& fileName)
{
	for (int i = 0; i < scene->mNumAnimations; ++i)
	{
		std::string animName = fileName + "/" + scene->mAnimations[i]->mName.C_Str();

		// Check to see if animation was already loaded
		Animation* anim = AssetManager::LoadAnimation(animName);

		if (!anim)
		{
			// Create a new animation
			anim = new Animation(scene->mAnimations[i], mSkeleton, animName);

			// Save the animation into AssetManager
			AssetManager::SaveAnimation(animName, anim);
		}

		// Add the anim to map
		mAnimations[animName] = anim;
	}
}

void AnimationComponent::Update(float deltaTime)
{
	if (mCurrentAnimation)
	{
		mCurrentTime += mCurrentAnimation->GetTicksPerSecond() * deltaTime;

		if (mCurrentTime >= mCurrentAnimation->GetDuration())
		{
			mCurrentTime = 0.0f;
		}
	}

	// Calculate final bone matrices on separate threads (go to AnimationComponent::UpdateBoneJob::DoJob() and paste here if you want single thread)
	JobManager::Get()->AddJob(&mJob);
}

void AnimationComponent::UpdateSkeletonBuffer()
{
	mSkeletonBuffer->UpdateBufferData(&mSkeletonConsts);
}

void AnimationComponent::SetCurrentAnimation(Animation* anim)
{
	if (anim)
	{
		mCurrentAnimation = anim;
		mCurrentTime = 0.0f;
	}
}

void AnimationComponent::SetCurrentAnimation(const std::string& name)
{
	auto iter = mAnimations.find(name);
	if (iter != mAnimations.end())
	{
		mCurrentAnimation = iter->second;
		mCurrentTime = 0.0f;
	}
}

void AnimationComponent::UpdateBoneJob::DoJob()
{
	if (mComp->mCurrentAnimation)
	{
		const std::vector<glm::mat4>& matrices = mComp->mSkeleton->GetPoseAtTime(mComp->mCurrentTime, mComp->mCurrentAnimation);

		for (size_t i = 0; i < matrices.size(); ++i)
		{
			mComp->mSkeletonConsts.finalBoneMatrices[i] = matrices[i];
		}
	}
	else
	{
		mComp->mSkeleton->GetBindPose();

		for (size_t i = 0; i < MAX_BONES; ++i)
		{
			mComp->mSkeletonConsts.finalBoneMatrices[i] = glm::mat4(1.0f);
		}
	}
}

void AnimationComponent::SetDefaultAnim()
{
	if (!mAnimations.empty())
	{
		mCurrentAnimation = mAnimations.begin()->second;
	}
}

void AnimationComponent::LoadCachedAnims(const Skeleton* skeleton)
{
	// Get the cached skeleton's anims
	const std::vector<Animation*>& anims = skeleton->GetAnims();

	for (auto a : anims)
	{
		// Add anim to map
		mAnimations[a->GetName()] = a;
	}
}
