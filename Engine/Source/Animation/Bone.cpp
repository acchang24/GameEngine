#include "Bone.h"
#include <iostream>
#include "../Util/AssimpGLMHelper.h"

Bone::Bone(const std::string& name, int id, const glm::mat4 offset, const aiNodeAnim* channel) :
	mLocalTransform(glm::mat4(1.0f)),
	mOffset(offset),
	mName(name),
	mID(id),
	mNumPositions(channel->mNumPositionKeys),
	mNumRotations(channel->mNumRotationKeys),
	mNumScalings(channel->mNumScalingKeys)
{
	mPositions.reserve(mNumPositions);
	for (int i = 0; i < mNumPositions; ++i)
	{
		aiVectorKey key = channel->mPositionKeys[i];
		aiVector3D aiPosition = key.mValue;
		float timeStamp = key.mTime;
		KeyPosition data = {};
		data.position = AssimpGLMHelper::GetGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		mPositions.emplace_back(data);
	}
	mRotations.reserve(mNumRotations);
	for (int i = 0; i < mNumRotations; ++i)
	{
		aiQuatKey key = channel->mRotationKeys[i];
		aiQuaternion aiRotation = key.mValue;
		float timeStamp = key.mTime;
		KeyRotation data = {};
		data.rotation = AssimpGLMHelper::GetGLMQuat(aiRotation);
		data.timeStamp = timeStamp;
		mRotations.emplace_back(data);
	}
	mScalings.reserve(mNumScalings);
	for (int i = 0; i < mNumScalings; ++i)
	{
		aiVectorKey key = channel->mScalingKeys[i];
		aiVector3D scale = key.mValue;
		float timeStamp = key.mTime;
		KeyScale data = {};
		data.scale = AssimpGLMHelper::GetGLMVec(scale);
		data.timeStamp = timeStamp;
		mScalings.emplace_back(data);
	}
}

Bone::~Bone()
{
	//std::cout << "Delete bone" << std::endl;
}

void Bone::Update(float animTime, float animDuration)
{
	glm::mat4 translation = InterpolatePosition(animTime, animDuration);
	glm::mat4 rotation = InterpolateRotation(animTime, animDuration);
	glm::mat4 scale = InterpolateScaling(animTime, animDuration);

	mLocalTransform = translation * rotation * scale;
}

glm::mat4 Bone::InterpolatePosition(float animTime, float animDuration) const
{
	if (mNumPositions == 1)
	{
		return glm::translate(glm::mat4(1.0f), mPositions[0].position);
	}
	float timePerFrame = animDuration / static_cast<float>(mNumPositions - 1);
	float frame = animTime / timePerFrame;
	int index = static_cast<int>(frame);
	int nextIndex = index + 1;

	glm::vec3 finalPosition = glm::mix(mPositions[index].position, mPositions[nextIndex].position, (frame - index));

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animTime, float animDuration) const
{
	if (mNumRotations == 1)
	{
		glm::quat rot = glm::normalize(mRotations[0].rotation);
		return glm::toMat4(rot);
	}
	float timePerFrame = animDuration / static_cast<float>(mNumRotations - 1);
	float frame = animTime / timePerFrame;
	int index = static_cast<int>(frame);
	int nextIndex = index + 1;

	glm::quat finalRotation = glm::slerp(mRotations[index].rotation, mRotations[nextIndex].rotation, (frame - index));
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(float animTime, float animDuration) const
{
	if (mNumScalings == 1)
	{
		return glm::scale(glm::mat4(1.0f), mScalings[0].scale);
	}
	float timePerFrame = animDuration / static_cast<float>(mNumScalings - 1);
	float frame = animTime / timePerFrame;
	int index = static_cast<int>(frame);
	int nextIndex = index + 1;

	glm::vec3 finalScale = glm::mix(mScalings[index].scale, mScalings[nextIndex].scale, (frame - index));

	return glm::scale(glm::mat4(1.0f), finalScale);
}
