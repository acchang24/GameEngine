#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "../Graphics/VertexLayouts.h"
#include "BoneData.h"
#include "Animation.h"

class UniformBuffer;

struct SkeletonConsts
{
	glm::mat4 BoneMatrices[100];
};

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	void ExtractVertexBoneWeights(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);


	void UpdateAnimation(float deltaTime);

	void CalculateBoneTransform(const AssimpNode* node, glm::mat4 parentTransform);



	std::unordered_map<std::string, BoneData>& GetBoneMap() { return mBoneMap; }

	int& GetNumBones() { return mNumBones; }

	std::vector<glm::mat4>& GetFinalBoneMatrices() { return mFinalBoneMatrices; }
	UniformBuffer* GetSkeletonBuffer() { return mSkeletonBuffer; }
	
	Animation* GetCurrentAnimation() { return mCurrentAnimation; }


	void AddAnimation(Animation* a)
	{
		if (mAnimations.find(a->GetName()) == mAnimations.end())
		{
			mAnimations[a->GetName()] = a;
		}
	}

	void SetAnimation(Animation* a)
	{
		mCurrentAnimation = a;
		mCurrentTime = 0.0f;
	}


private:
	std::unordered_map<std::string, BoneData> mBoneMap;
	std::unordered_map<std::string, Animation*> mAnimations;

	std::vector<glm::mat4> mFinalBoneMatrices;

	UniformBuffer* mSkeletonBuffer;

	Animation* mCurrentAnimation;
	float mCurrentTime;
	float mDeltaTime;

	int mNumBones;
};