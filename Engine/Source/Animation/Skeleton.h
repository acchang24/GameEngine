#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "../Graphics/UniformBuffer.h"
#include "../Graphics/VertexLayouts.h"
#include "../Multithreading/JobManager.h"
#include "BoneData.h"
#include "Animation.h"

// Max number of bones a skeleton can have
const int MAX_BONES = 100;

// Struct for the skeleton constants to send to a buffer
// in a shader. Holds an array of MAX_BONES bone matrices used
// for the skeleton's final bone pose.
struct SkeletonConsts
{
	glm::mat4 finalBoneMatrices[MAX_BONES];
};

// Skeleton class is used to hold bone and animation data.
// This helps skin a 3d model to be able to update its animations.
class Skeleton
{
	// Allow Animation class to access private Skeleton functions
	friend class Animation;

public:
	Skeleton();
	~Skeleton();
	Skeleton(Skeleton& other);

	// Loops through the assimp mesh's bones and loads the bone's id and inverse bind pose matrices
	// into the skeleton's bone map
	// @param - const aiMesh* for the mesh being processed
	void LoadBoneData(const aiMesh* mesh);

	// Loops through the assimp mesh's bones and extracts that bone's ids and weights and sets them to the vertex
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

	// Takes the animation's root node and calculates the final bone transformations on each bone
	// @param - const AnimNode* for the animation's root node
	// @param - const glm::mat4& for the bone's transformation relative to its parent
	void CalculateBoneTransform(const AnimNode* node, const glm::mat4& parentTransform);

	// Updates the skeleton buffer with the skeleton's final bone matrices
	void UpdateSkeletonBuffer() { mSkeletonBuffer->UpdateBufferData(mSkeletonConsts.finalBoneMatrices); }
	
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

private:
	// Job to update bone matrices on a separate thread
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


	// MEMBER VARIABLES
	// Map of the skeleton's bones
	std::unordered_map<std::string, BoneData> mBoneMap;

	// Map of the skeleton's animations
	std::unordered_map<std::string, Animation*> mAnimations;

	// Skeleton consts to send to buffers in shaders
	SkeletonConsts mSkeletonConsts;

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
