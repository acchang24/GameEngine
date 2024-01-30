#include "Skeleton.h"
#include <iostream>
#include "../Util/AssimpGLMHelper.h"
#include "../MemoryManager/AssetManager.h"
#include "../Graphics/UniformBuffer.h"

Skeleton::Skeleton() :
	mGlobalInverseTransform(glm::mat4(1.0f)),
	mSkeletonBuffer(AssetManager::Get()->LoadBuffer("SkeletonBuffer")),
	mCurrentAnimation(nullptr),
	mJob(this),
	mCurrentTime(0.0f),
	mNumBones(0)
{
	mFinalBoneMatrices.reserve(MAX_BONES);
	for (int i = 0; i < MAX_BONES; ++i)
	{
		mFinalBoneMatrices.emplace_back(glm::mat4(1.0f));
	}
}

Skeleton::~Skeleton()
{
	std::cout << "Delete skeleton" << std::endl;

	for (auto& a : mAnimations)
	{
		delete a.second;
	}
	mAnimations.clear();
}

void Skeleton::LoadBoneData(const aiMesh* mesh)
{
	// The bone's name
	std::string boneName;

	// Loop through the assimp mesh's bones
	for (int i = 0; i < mesh->mNumBones; ++i)
	{
		boneName = mesh->mBones[i]->mName.C_Str();

		// Bone's ID
		int boneID = i;

		// Check to see if bone is in the map and set the boneID to the one in the index
		// If there is no bone, make a new bone, add it to the bone map
		if (mBoneMap.find(boneName) == mBoneMap.end())
		{
			BoneData newBone = {};
			newBone.index = i;
			newBone.offset = AssimpGLMHelper::ConvertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix);
			mBoneMap[boneName] = newBone;
		}
	}
}

void Skeleton::ExtractVertexBoneWeights(std::vector<Vertex>& vertices, const aiMesh* mesh)
{
	// The bone's name
	std::string boneName;

	// Loop through the assimp mesh's bones
	for (int i = 0; i < mesh->mNumBones; ++i)
	{
		boneName = mesh->mBones[i]->mName.C_Str();

		// Bone's ID
		int boneID = i;

		// Check to see if bone is in the map and set the boneID to the one in the index
		if (mBoneMap.find(boneName) != mBoneMap.end())
		{
			boneID = mBoneMap[boneName].index;
		}

		// Bone's array of bone weights
		aiVertexWeight* weightArray = mesh->mBones[i]->mWeights;
		// Number of weights for this bone
		int numWeights = mesh->mBones[i]->mNumWeights;

		// Loop through the bone's weights and set the vertex's bone data
		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
		{
			int vertexID = weightArray[weightIndex].mVertexId;
			float weight = weightArray[weightIndex].mWeight;

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

		// Update bone transformations on separate thread
		JobManager::Get()->AddJob(&mJob);

		//CalculateBoneTransform(&mCurrentAnimation->GetRootNode(), glm::mat4(1.0f));
	}
}

void Skeleton::CalculateBoneTransform(const AnimNode* node, const glm::mat4& parentTransform)
{
	std::string nodeName = node->name;

	glm::mat4 nodeTransform = node->transformation;

	Bone* bone = mCurrentAnimation->FindBone(nodeName);

	glm::mat4 globalTransformation(1.0f);

	if (bone)
	{
		bone->Update(mCurrentTime);

		nodeTransform = bone->GetLocalTransform();

		globalTransformation = parentTransform * nodeTransform;

		mFinalBoneMatrices[bone->GetBoneID()] = mGlobalInverseTransform * globalTransformation * bone->GetOffetMatrix();
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
	const AnimNode* root = &mSkeleton->GetCurrentAnimation()->GetRootNode();

	glm::mat4 parent(1.0f);

	mSkeleton->CalculateBoneTransform(root, parent);
}
