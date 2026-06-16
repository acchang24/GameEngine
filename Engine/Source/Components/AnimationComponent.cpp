#include "AnimationComponent.h"
#include <iostream>
#include "../Animation/Skeleton.h"
#include "../Animation/Animation.h"
#include "../Graphics/UniformBuffer.h"

AnimationComponent::AnimationComponent(Entity* entity, Skeleton* skeleton, UniformBuffer* skeletonBuffer) :
	Component(entity),
	mSkeletonConsts({}),
	mSkeleton(skeleton),
	mCurrentAnimation(nullptr),
	mSkeletonBuffer(skeletonBuffer),
	mJob(this),
	mCurrentTime(0.0f)
{
	if (skeleton)
	{
		for (Animation* anim : skeleton->GetAnims())
		{
			std::string animName = anim->GetName();

			if (anim)
			{
				mAnimations[animName] = anim;
			}
		}

		// Set default animation to first anim in map
		SetDefaultAnim();
	}
}

AnimationComponent::~AnimationComponent()
{
	std::cout << "Deleted animation component\n";
}

void AnimationComponent::Update(float deltaTime, const EngineContext& engineContext)
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
	engineContext.jobManager->AddJob(&mJob);
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
