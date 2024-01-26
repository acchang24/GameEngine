#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Skeleton.h"
#include "Bone.h"

class Skeleton;

struct AssimpNode
{
	glm::mat4 transformation;
	std::string name;
	std::vector<AssimpNode> children;
	int numChildren;
};

class Animation
{
public:
	Animation(const std::string& animPath, Skeleton* skeleton);
	~Animation();

	void ReadNodeHeirarchy(AssimpNode& dest, const aiNode* src);

	void ReadMissingBones(const aiAnimation* anim, Skeleton* skeleton);

	const std::unordered_map<std::string, BoneData>& GetBoneInfoMap() { return mBoneInfoMap; }

	Bone* FindBone(const std::string& name)
	{
		auto iter = std::find_if(mBones.begin(), mBones.end(),
			[&](const Bone& Bone)
			{
				return Bone.GetBoneName() == name;
			}
		);
		if (iter == mBones.end()) return nullptr;
		else return &(*iter);
	}

	const AssimpNode& GetRootNode() const { return mRoot; }
	float GetDuration() const { return mDuration; }
	float GetTicksPerSecond() const { return mTicksPerSecond; }

private:
	std::unordered_map<std::string, BoneData> mBoneInfoMap;
	std::vector<Bone> mBones;

	AssimpNode mRoot;
	
	float mDuration;

	float mTicksPerSecond;
};