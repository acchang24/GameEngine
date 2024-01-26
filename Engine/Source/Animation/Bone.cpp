#include "Bone.h"
#include <iostream>
#include "../Util/AssimpGLMHelper.h"

Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel) :
	mLocalTransform(glm::mat4(1.0f)),
	mName(name),
	mID(id),
	mNumPositions(channel->mNumPositionKeys),
	mNumRotations(channel->mNumRotationKeys),
	mNumScalings(channel->mNumScalingKeys)
{
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
	std::cout << "Delete bone" << std::endl;
}

void Bone::Update(float animTime)
{
	glm::mat4 translation = InterpolatePosition(animTime);
	glm::mat4 rotation = InterpolateRotation(animTime);
	glm::mat4 scale = InterpolateScaling(animTime);

	mLocalTransform = translation * rotation * scale;
}

int Bone::GetPositionIndex(float animTime) const
{
	for (int i = 0; i < mNumPositions - 1; ++i)
	{
		if (animTime < mPositions[static_cast<size_t>(i + 1)].timeStamp)
		{
			return i;
		}
	}
	return 0;
}

int Bone::GetRotationIndex(float animTime) const
{
	for (int i = 0; i < mNumRotations - 1; ++i)
	{
		if (animTime < mRotations[static_cast<size_t>(i + 1)].timeStamp)
		{
			return i;
		}
	}
	return 0;
}

int Bone::GetScaleIndex(float animTime) const
{
	for (int i = 0; i < mNumScalings - 1; ++i)
	{
		if (animTime < mScalings[static_cast<size_t>(i + 1)].timeStamp)
		{
			return i;
		}
	}
	return 0;
}

float Bone::GetScaleFactor(float currFrame, float nextFrame, float animTime) const
{
	float scaleFactor = 0.0f;
	float mid = animTime - currFrame;
	float diff = nextFrame - currFrame;
	scaleFactor = mid / diff;

	return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animTime) const
{
	if (mNumPositions == 1)
	{
		return glm::translate(glm::mat4(1.0f), mPositions[0].position);
	}
	int index = GetPositionIndex(animTime);
	int nextIndex = index + 1;
	float scaleFactor = GetScaleFactor(mPositions[index].timeStamp, mPositions[nextIndex].timeStamp, animTime);
	glm::vec3 finalPosition = glm::mix(mPositions[index].position, mPositions[nextIndex].position, scaleFactor);

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animTime) const
{
	if (mNumRotations == 1)
	{
		glm::quat rot = glm::normalize(mRotations[0].rotation);
		return glm::toMat4(rot);
	}
	int index = GetRotationIndex(animTime);
	int nextIndex = index + 1;
	float scaleFactor = GetScaleFactor(mRotations[index].timeStamp, mRotations[nextIndex].timeStamp, animTime);
	glm::quat finalRotation = glm::slerp(mRotations[index].rotation, mRotations[nextIndex].rotation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::InterpolateScaling(float animTime) const
{
	if (mNumScalings == 1)
	{
		return glm::scale(glm::mat4(1.0f), mScalings[0].scale);
	}
	int index = GetScaleIndex(animTime);
	int nextIndex = index + 1;
	float scaleFactor = GetScaleFactor(mScalings[index].timeStamp, mScalings[nextIndex].timeStamp, animTime);
	glm::vec3 finalScale = glm::mix(mScalings[index].scale, mScalings[nextIndex].scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}
