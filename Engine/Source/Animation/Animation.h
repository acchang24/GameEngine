#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "BoneData.h"

class Skeleton;
class Bone;

// Struct to define a node that holds animation data about a bone and its bone heirarchy
struct AnimNode
{
	glm::mat4 transformation = glm::mat4(1.0f); // Transformation relative to the parent node/bone
	std::string name; // The node/bone's name
	std::vector<AnimNode> children; // The array of children node/bones
	Bone* bone = nullptr; // Bone if it exists in the model's skeleton
	int numChildren = 0; // Number of children
};

class Animation
{
public:
	// Animation constructor takes in an assimp animation, an assimp scene's root node, and a pointer to a Skeleton.
	// It adds this animation to the skeleton's map of animations as well as calculates the skeleton's 
	// global inverse transform matrix. It then reads in all the animation nodes' data and reads in the bone's keyframes.
	// Each animation node may or may not affect a bone within the model's skeleton of bones (number of nodes may be different from total bone count)
	// @param - const aiAnimation* for the assimp animation
	// @param - const aiNode* for the root node of the animation's bone heirarchy
	// @param - Skeleton* to add this animation to
	Animation(const aiAnimation* animation, const aiNode* rootNode, Skeleton* skeleton);
	~Animation();

	// Recursively reads through all the assimp animation nodes and saves them into an AnimNode.
	// A node will create a new bone if it exists within the model's skeleton bones (BoneData hash map loaded within the Skeleton class)
	// and adds it to the animation's vector of AnimNodes.
	// @param - AnimNode& for the animation's root node
	// @param - const aiNode* for the assimp root node
	// @param - std::unordered_map<std::string, BoneData>& for the model's Skeleton bone info map
	// @param - std::vector<Bone*>& for a reference to an array of bones to later load in keyframes
	void ReadNodeHeirarchy(AnimNode& dest, const aiNode* src, std::unordered_map<std::string, BoneData>& boneData, std::vector<Bone*>& bones);

	// Takes in an assimp animation and loops through all of its channels and reads all of the bone's key frames.
	// @param - const aiAnimation* for the assimp animation
	// @param - std::vector<Bone*>& for all the bones that need its key frames loaded
	void ReadBones(const aiAnimation* anim, std::vector<Bone*>& bones);

	// Gets the animation node data
	// @return - std::vector<AnimNode>& for the animation's data
	std::vector<AnimNode>& GetAnimData() { return mData; }

	// Gets the animation's root node
	// @return - const AnimNode& for the animation's root node
	const AnimNode& GetRootNode() const { return mRoot; }

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
	// Array of animation data
	std::vector<AnimNode> mData;

	// The animation's root node
	AnimNode mRoot;
	
	// The animation's name
	std::string mName;

	// The duration of this animation
	float mDuration;

	// The ticks per second for this animation
	float mTicksPerSecond;
};
