#include "Skeleton.h"
#include <iostream>
#include "../Graphics/Renderer3D.h"
#include "../MemoryManager/AssetManager.h"
#include "../Util/AssimpGLMHelper.h"
#include "Animation.h"

Skeleton::Skeleton(const aiScene* scene) :
	mRootInverseTransform(glm::inverse(AssimpGLMHelper::ConvertMatrixToGLMFormat(scene->mRootNode->mTransformation)))
{
	BuildHierarchy(scene->mRootNode, -1);
	LoadOffset(scene);
}

Skeleton::~Skeleton()
{
	std::cout << "Deleted Skeleton\n";
}

const std::vector<glm::mat4> Skeleton::GetPoseAtTime(float animTime, const Animation* currAnim)
{
	// Temp vector for global transforms built from chaining the parent bone
	std::vector<glm::mat4> globalTransforms(mBones.size(), glm::mat4(1.0f));

	// Vector for final bone matrices
	std::vector<glm::mat4> finalMatrices;
	finalMatrices.resize(mBones.size());

	// Loop through the skeleton's bones
	for (size_t i = 0; i < mBones.size(); ++i)
	{
		const Bone& bone = mBones[i];

		// Set the bone's local transform to itself (this will change if there is an animation)
		glm::mat4 localTransform = bone.localTransform;

		// Check if there is an animation
		if (currAnim)
		{
			// Get the bone's animation track
			const AnimationTrack* track = currAnim->GetTrack(static_cast<int>(i));

			// If the track exists, calculate the local transform for the bone at the time
			if (track)
			{
				glm::mat4 translation = currAnim->InterpolatePosition(animTime, *track);
				glm::mat4 rotation = currAnim->InterpolateRotation(animTime, *track);
				glm::mat4 scaling = currAnim->InterpolateScaling(animTime, *track);

				localTransform = translation * rotation * scaling;
			}
		}

		// Compute global transforms
		if (bone.parentID == -1)
		{
			// Root bone
			globalTransforms[i] = mRootInverseTransform * localTransform;
		}
		else
		{
			// Multiply by the global transform of the parent bone with the bone's local transform to chain the global transform
			globalTransforms[i] = globalTransforms[bone.parentID] * localTransform;
		}

		// If the bone influences a mesh and not the root bone, set the final bone matrix to the global transform times the bone's inverse bind pose
		if (bone.influenceMesh && bone.id >= 0)
		{
			finalMatrices[bone.id] = globalTransforms[i] * bone.offsetMatrix;
		}
	}

	return finalMatrices;
}

int Skeleton::GetBoneID(const std::string& name) const
{
	auto iter = mBoneNameToID.find(name);

	if (iter != mBoneNameToID.end())
	{
		return iter->second;
	}

	return -1;
}

void Skeleton::BuildHierarchy(const aiNode* node, int parentID)
{
	// Bone name
	std::string name = node->mName.C_Str();

	int boneID = -1;

	// See if bone is in the map
	auto iter = mBoneNameToID.find(name);
	if (iter == mBoneNameToID.end())
	{
		// No bone found

		// Use the bone vector size as id (bone number is in hierarchy order)
		boneID = static_cast<int>(mBones.size());

		// Create a new bone
		Bone bone = {};
		bone.localTransform = AssimpGLMHelper::ConvertMatrixToGLMFormat(node->mTransformation);
		bone.name = name;
		bone.id = boneID;
		bone.parentID = parentID;

		mBoneNameToID[name] = boneID;

		mBones.emplace_back(bone);
	}
	else
	{
		// Bone found (unlikely)
		// set boneID to the found bone's id
		boneID = iter->second;
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		BuildHierarchy(node->mChildren[i], boneID);
	}
}

void Skeleton::LoadOffset(const aiScene* scene)
{
	// Loop through the meshes
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[i];

		// Loop through the mesh's bones
		for(unsigned int j = 0; j < mesh->mNumBones; ++j)
		{
			aiBone* bone = mesh->mBones[j];

			std::string name = bone->mName.C_Str();

			// Get the bone by index
			int index = GetBoneID(name);

			Bone& b = mBones[index];

			b.offsetMatrix = AssimpGLMHelper::ConvertMatrixToGLMFormat(bone->mOffsetMatrix);
			b.influenceMesh = true;
		}
	}
}
