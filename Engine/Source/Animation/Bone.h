#pragma once
#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat rotation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	// Bone constructor reads in keyframes from an aiNodeAnim and saves
	// all position, rotation, scalings in a vector as its key frame count
	Bone(const std::string& name, int id, const aiNodeAnim* channel);
	~Bone();

	void Update(float animTime);

	int GetPositionIndex(float animTime) const;
	int GetRotationIndex(float animTime) const;
	int GetScaleIndex(float animTime) const;

	float GetScaleFactor(float currFrame, float nextFrame, float animTime) const;

	glm::mat4 InterpolatePosition(float animTime) const;

	glm::mat4 InterpolateRotation(float animTime) const;

	glm::mat4 InterpolateScaling(float animTime) const;

	const glm::mat4& GetLocalTransform() const { return mLocalTransform; }

	const std::string& GetBoneName() const { return mName; }

	int GetBoneID() const { return mID; }

private:
	std::vector<KeyPosition> mPositions;
	std::vector<KeyRotation> mRotations;
	std::vector<KeyScale> mScalings;

	glm::mat4 mLocalTransform;

	std::string mName;

	int mID;

	int mNumPositions;
	int mNumRotations;
	int mNumScalings;
};
