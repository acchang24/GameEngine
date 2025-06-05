#include "Animation.h"
#include <iostream>
#include "../Util/AssimpGLMHelper.h"
#include "Skeleton.h"

Animation::Animation(const aiAnimation* anim, Skeleton* skeleton, const std::string& animName) :
	mName(animName),
	mDuration(static_cast<float>(anim->mDuration)),
	mTicksPerSecond(static_cast<float>(anim->mTicksPerSecond))
{
	skeleton->AddAnim(this);

	ReadKeyFrames(anim, skeleton);
}

Animation::~Animation()
{
	std::cout << "Deleted Animation: " << mName << "\n";
}

const AnimationTrack* Animation::GetTrack(int boneID) const
{
	auto iter = mAnimationTracks.find(boneID);
	if (iter != mAnimationTracks.end())
	{
		return &iter->second;
	}
	return nullptr;
}

glm::mat4 Animation::InterpolatePosition(float currentTime, const AnimationTrack& track) const
{
	if (track.positions.empty())
	{
		return glm::mat4(1.0f);
	}

	if (track.positions.size() == 1)
	{
		return glm::translate(glm::mat4(1.0f), track.positions[0].position);
	}

	float timePerFrame = mDuration / static_cast<float>(track.positions.size() - 1);
	float frame = currentTime / timePerFrame;
	int index = static_cast<int>(frame);
	int nextIndex = index + 1;

	glm::vec3 finalPosition = glm::mix(track.positions[index].position, track.positions[nextIndex].position, (frame - index));

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Animation::InterpolateRotation(float currentTime, const AnimationTrack& track) const
{
	if (track.rotations.empty())
	{
		return glm::mat4(1.0f);
	}

	if (track.rotations.size() == 1)
	{
		glm::quat rot = glm::normalize(track.rotations[0].rotation);
		return glm::toMat4(rot);
	}

	float timePerFrame = mDuration / static_cast<float>(track.rotations.size() - 1);
	float frame = currentTime / timePerFrame;
	int index = static_cast<int>(frame);
	int nextIndex = index + 1;

	glm::quat finalRotation = glm::slerp(track.rotations[index].rotation, track.rotations[nextIndex].rotation, (frame - index));
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Animation::InterpolateScaling(float currentTime, const AnimationTrack& track) const
{
	if (track.scalings.empty())
	{
		return glm::mat4(1.0f);
	}

	if (track.scalings.size() == 1)
	{
		return glm::scale(glm::mat4(1.0f), track.scalings[0].scale);
	}

	float timePerFrame = mDuration / static_cast<float>(track.scalings.size() - 1);
	float frame = currentTime / timePerFrame;
	int index = static_cast<int>(frame);
	int nextIndex = index + 1;

	glm::vec3 finalScale = glm::mix(track.scalings[index].scale, track.scalings[nextIndex].scale, (frame - index));

	return glm::scale(glm::mat4(1.0f), finalScale);
}

void Animation::ReadKeyFrames(const aiAnimation* anim, const Skeleton* skeleton)
{
	// Loop through the animation's channels
	for (unsigned int i = 0; i < anim->mNumChannels; ++i)
	{
		aiNodeAnim* channel = anim->mChannels[i];

		// Create a track for each channel/bone
		AnimationTrack track = {};

		// Get the bone's id from the skeleton
		int boneID = skeleton->GetBoneID(channel->mNodeName.C_Str());
		if (boneID >= 0)
		{
			// Position keys
			track.positions.reserve(channel->mNumPositionKeys);
			for (unsigned int j = 0; j < channel->mNumPositionKeys; ++j)
			{
				const aiVectorKey& key = channel->mPositionKeys[j];
				KeyPosition posKey = { AssimpGLMHelper::GetGLMVec(key.mValue), static_cast<float>(key.mTime) };
				track.positions.emplace_back(posKey);
			}

			// Rotation keys
			track.rotations.reserve(channel->mNumRotationKeys);
			for (unsigned int j = 0; j < channel->mNumRotationKeys; ++j)
			{
				const aiQuatKey& key = channel->mRotationKeys[j];
				KeyRotation rotKey = { AssimpGLMHelper::GetGLMQuat(key.mValue), static_cast<float>(key.mTime) };
				track.rotations.emplace_back(rotKey);
			}

			// Scaling keys
			track.scalings.reserve(channel->mNumScalingKeys);
			for (unsigned int j = 0; j < channel->mNumScalingKeys; ++j)
			{
				const aiVectorKey& key = channel->mScalingKeys[j];
				KeyScale scaleKey = { AssimpGLMHelper::GetGLMVec(key.mValue), static_cast<float>(key.mTime) };
				track.scalings.emplace_back(scaleKey);
			}

			// Save the bone's animation track using its ID
			mAnimationTracks[boneID] = track;
		}
	}
}
