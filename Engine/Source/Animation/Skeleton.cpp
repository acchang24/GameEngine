#include "Skeleton.h"
#include <iostream>
#include "../Util/AssimpGLMHelper.h"
#include "../MemoryManager/AssetManager.h"
#include "Bone.h"

Skeleton::Skeleton() :
	mSkeletonConsts({}),
	mJob(this),
	mSkeletonBuffer(AssetManager::Get()->LoadBuffer("SkeletonBuffer")),
	mCurrentAnimation(nullptr),
	mCurrentTime(0.0f),
	mNumBones(0)
{
}

Skeleton::~Skeleton()
{
	mBoneDataMap.clear();

	mAnimations.clear();

	std::cout << "Deleted skeleton\n";
}

Skeleton::Skeleton(Skeleton& other) :
	mBoneDataMap(other.mBoneDataMap),
	mAnimations(other.mAnimations),
	mSkeletonConsts({}),
	mJob(this),
	mSkeletonBuffer(other.mSkeletonBuffer),
	mCurrentAnimation(other.mCurrentAnimation),
	mCurrentTime(other.mCurrentTime),
	mNumBones(other.mNumBones)
{
}

void Skeleton::LoadBoneData(const aiMesh* mesh)
{
	std::string boneName;

	for (int i = 0; i < mesh->mNumBones; ++i)
	{
		boneName = mesh->mBones[i]->mName.C_Str();

		if (mBoneDataMap.find(boneName) == mBoneDataMap.end())
		{
			BoneData newBone = {};
			newBone.index = i;
			newBone.offset = AssimpGLMHelper::ConvertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix);
			mBoneDataMap[boneName] = newBone;
		}
	}
}

void Skeleton::ExtractVertexBoneWeights(std::vector<Vertex>& vertices, const aiMesh* mesh) const
{
	for (int i = 0; i < mesh->mNumBones; ++i)
	{
		int boneID = i;

		aiVertexWeight* boneWeightsArray = mesh->mBones[i]->mWeights;

		int numWeights = mesh->mBones[i]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexID = boneWeightsArray[weightIndex].mVertexId;
			float weight = boneWeightsArray[weightIndex].mWeight;

			SetVertexBoneData(vertices[vertexID], boneID, weight);
		}
	}
}

void Skeleton::SetVertexBoneData(Vertex& vertex, int boneID, float weight) const
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.boneIDs[i] < 0)
		{
			vertex.boneIDs[i] = boneID;
			vertex.weights[i] = weight;
			break;
		}
	}
}

void Skeleton::UpdateAnimation(float deltaTime)
{
	if (mCurrentAnimation)
	{
		mCurrentTime += mCurrentAnimation->GetTicksPerSecond() * deltaTime;
		if (mCurrentTime > mCurrentAnimation->GetDuration())
		{
			mCurrentTime = 0.0f;
		}

		// Updates bone transformations on separate thread
		JobManager::Get()->AddJob(&mJob);
		
		// Uncomment this and remove/comment out the JobManager::Get()->AddJob() function the line above to use single thread
		//CalculateBoneTransform(&mCurrentAnimation->GetRootNode(), GetCurrentAnimation()->GetGlobalInverseTransform());
	}
}

void Skeleton::CalculateBoneTransform(const AnimNode* node, const glm::mat4& parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;
	Bone* bone = node->bone;
	glm::mat4 globalTransformation(1.0f);
	if (bone)
	{
		bone->Update(mCurrentTime, mCurrentAnimation->GetDuration());
		nodeTransform = bone->GetLocalTransform();
		globalTransformation = parentTransform * nodeTransform;
		mSkeletonConsts.finalBoneMatrices[bone->GetBoneID()] = globalTransformation * bone->GetOffetMatrix();
	}
	else
	{
		globalTransformation = parentTransform * nodeTransform;
	}

	for (int i = 0; i < node->numChildren; ++i)
	{
		CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}

void Skeleton::UpdateBoneJob::DoIt()
{
	mSkeleton->CalculateBoneTransform(&mSkeleton->GetCurrentAnimation()->GetRootNode(), mSkeleton->GetCurrentAnimation()->GetGlobalInverseTransform());
}
