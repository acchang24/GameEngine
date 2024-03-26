#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "BoneData.h"

class Bone;
class Skeleton;

// AnimNode is a node that holds animation data about a bone and its bone heirarchy
struct AnimNode
{
	glm::mat4 transformation; // Transformation relative to the parent node/bone
	std::string name; // The node/bone's name
	std::vector<AnimNode> children; // The array of children nodes/bones
	Bone* bone = nullptr; // Bone if it exists in the model's skeleton
	int numChildren; // Number of children
};

class Animation
{
public:
	// Animation constructor takes in an assimp animation, an assimp scene's root node, and a pointer to a Skeleton.
	// It adds this animation to the skeleton's map of animations as well as calculates its global inverse
	// transform matrix. It then reads in all the animation nodes' data and reads in the bone's keyframes.
	// Each animation node may or may not affect a bone within the model's skeleton of bones (number of nodes may be different from total bone count)
	// @param - const aiAnimation* for the assimp animation
	// @param - const aiNode* for the root node of the animation's bone heirarchy
	// @param - Skeleton* to add this animation to
	Animation(const aiAnimation* animation, const aiNode* rootNode, Skeleton* skeleton);
	~Animation();

	// Recursively reads through all the assimp animation nodes and saves them into an AnimNode.
	// A node will create a new bone if it exists within the model's skeleton bones (BoneData hash map loaded within the Skeleton class)
	// and adds it to the animation's vector of AnimNodes.
	// @param - AnimNode& for the current node that is being processed
	// @param - const aiNode* for the assimp root node
	// @param - std::unordered_map<std::string, BoneData>& for the model's Skeleton bone info map
	void ReadNodeHeirarchy(AnimNode& curr, const aiNode* src, std::unordered_map<std::string, BoneData>& boneData);

	// Takes in an assimp animation and loops through all of its channels and reads all of the bone's key frames.
	// @param - const aiAnimation* for the assimp animation
	void ReadBones(const aiAnimation* anim);

	// Gets the animation's root node
	// @return - const AnimNode& for the animation's root node
	const AnimNode& GetRootNode() const { return mRoot; }

	// Gets the animation's global inverse transform matrix
	// @return - const glm::mat4& for the animation's global inverse transform matrix
	const glm::mat4& GetGlobalInverseTransform() const { return mGlobalInverseTransform; }

	// Gets the animation's name
	// @return - const std::string& for the animation's name
	const std::string& GetName() const { return mName; }
	
	// Gets the animation's duration
	// @return - float for the animation's duration
	float GetDuration() const { return mDuration; }

	// Gets the ticks per second for this animation
	// @return - float for the ticks per second
	float GetTicksPerSecond() const { return mTicksPerSecond; }

private:
	// Array of bones affected by this animation
	std::vector<Bone*> mBones;

	// The animation's root node
	AnimNode mRoot;
	
	// Global inverse transform matrix
	glm::mat4 mGlobalInverseTransform;

	// The animation's name
	std::string mName;

	// The duration of this animation
	float mDuration;

	// The ticks per second for this animation
	float mTicksPerSecond;
};
