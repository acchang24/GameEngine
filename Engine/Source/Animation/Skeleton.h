#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "../Graphics/VertexLayouts.h"
#include "BoneData.h"
#include "Animation.h"
#include "../Multithreading/JobManager.h"

const int MAX_BONES = 100;

// Struct for the skeleton constants to send to a buffer
// in a shader. Holds an array of 100 bone matrices used
// for the skeleton's final bone pose.
struct SkeletonConsts
{
	glm::mat4 finalBoneMatrices[MAX_BONES];
};

class UniformBuffer;

// Skeleton class is used to hold bone and animation data.
// This helps skin a 3d entity to be able to update its animations.
class Skeleton
{
	friend class Animation;

public:
	Skeleton();
	~Skeleton();

	// Loops through the assimp mesh's bones and extracts the bone's id and inverse bind pose matrices.
	// It then extracts that bone's weights and sets them to the vertex's bone ids and weight arrays.
	// @param - std::vector<Vertex>& for the array of vertices used to store the bone id and weights
	// @param - const aiMesh* for the mesh being processed
	void ExtractVertexBoneWeights(std::vector<Vertex>& vertices, const aiMesh* mesh);

	// Sets the vertex's bone id and bone weights
	// @param - Vertex& for the vertex
	// @param - the bone's id
	// @param - the bone's weight
	void SetVertexBoneData(Vertex& vertex, int boneID, float weight) const;

	// If an animation exists, increment the animation's current time. Resets the anim time whenver
	// it reaches the animation's duration. It finally calculate the final bone transformation matrices
	// @param - float for delta time
	void UpdateAnimation(float deltaTime);

	// Takes the animation's root node and calculates the bone transformations on each bone
	// @param - const AnimNode* for the animation's root node
	// @param - const glm::mat4& for the bone's transformation relative to its parent
	void CalculateBoneTransform(const AnimNode* node, const glm::mat4& parentTransform);

	// Gets the array of final bone matrices to send to a shader
	// @return - std::vector<glm::mat4>& for the bone matrix array
	std::vector<glm::mat4>& GetFinalBoneMatrices() { return mFinalBoneMatrices; }

	// Gets the skeleton buffer
	// @return - UniformBuffer* for the skeleton's buffer
	UniformBuffer* GetSkeletonBuffer() { return mSkeletonBuffer; }
	
	// Gets the skeleton's current animation
	// @return - Animation* for the skeleton's current animation
	Animation* GetCurrentAnimation() { return mCurrentAnimation; }

	// Adds an animation to the skelton's map of animations if it doesn't exist
	// @param - Animation* for the new animation
	void AddAnimation(Animation* a)
	{
		std::string animName = a->GetName();
		if (mAnimations.find(animName) == mAnimations.end())
		{
			mAnimations[animName] = a;
		}
	}

	// Sets the current animation and resets the current time to 0
	// @param - Animation* for the new animation
	void SetAnimation(Animation* a)
	{
		mCurrentAnimation = a;
		mCurrentTime = 0.0f;
	}

	// Sets the global inverse transform matrix
	// @param - const glm::mat4& for the transformation matrix
	void SetGlobalInverseTransform(const glm::mat4& transform) { mGlobalInverseTransform = transform; }

private:
	// Job to update bone matrices
	class UpdateBoneJob : public JobManager::Job
	{
	public:
		UpdateBoneJob(Skeleton* s) : mSkeleton(s)
		{}

		void DoIt() override;

	private:
		Skeleton* mSkeleton;
	};

	// Gets the reference to the skeleton's bone map (can change)
	// @return - std::unordered_map<std::string, BoneData>& for the skeleton's bone data map
	std::unordered_map<std::string, BoneData>& GetBoneMap() { return mBoneMap; }

	// Gets the reference to the skeleton's bone count (can change)
	// @return - int for the number of bones in the skeleton
	int& GetNumBones() { return mNumBones; }

	// Map of the skeleton's bones
	std::unordered_map<std::string, BoneData> mBoneMap;

	// Map of the skeleton's animations
	std::unordered_map<std::string, Animation*> mAnimations;

	// Array of the skeleton's final bone matrices
	std::vector<glm::mat4> mFinalBoneMatrices;

	glm::mat4 mGlobalInverseTransform;

	// Uniform buffer to send the skeleton's bone matrices to a shader
	UniformBuffer* mSkeletonBuffer;

	// The skeleton's current animation
	Animation* mCurrentAnimation;

	// Job to update bone matrices
	UpdateBoneJob mJob;

	// The current time elapsed for the animation
	float mCurrentTime;

	// The number of bones this skeleton has
	int mNumBones;
};