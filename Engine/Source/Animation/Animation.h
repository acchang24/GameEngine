#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "Bone.h"
#include "BoneData.h"

class Skeleton;

// Struct to define an animation node that holds animation information
struct AnimNode
{
	glm::mat4 transformation = glm::mat4(1.0f); // Transformation relative to the parent
	std::string name;
	std::vector<AnimNode> children;
	Bone* bone = nullptr;
	int numChildren = 0;
};

class Animation
{
public:
	// Animation constructor takes in an assimp animation, assimp scene's root node, and a skeleton.
	// It adds this animation to the skeleton's map of animations as well as calculates the skeleton's 
	// global inverse transform matrix. It then reads in all the animation nodes and reads in any missing
	// bones and saves it into the skeleton's map of bones
	Animation(const aiAnimation* animation, const aiNode* rootNode, Skeleton* skeleton);
	~Animation();

	// Recursively reads through all the assimp animation nodes and sets them into an AnimNode
	// @param - AnimNode& for the animation's root node
	// @param - const aiNode* for the assimp root node
	void ReadNodeHeirarchy(AnimNode& dest, const aiNode* src, std::unordered_map<std::string, BoneData>& boneData);

	// Takes in an assimp animation, loops through all the channels and creates
	// a Bone. If there are extra bones in the animation that weren't parsed for the
	// skeleton, it will add those missing bones to the skeleton's map of bones. It then 
	// sets the animation's bone map to the skeleton's bone map
	void ReadBones(const aiAnimation* anim, Skeleton* skeleton);

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

	// Gets the animation node data
	// @return - std::vector<AnimNode>& for the animation's data
	std::vector<AnimNode>& GetAnimData() { return mData; }

private:
	// Array of bones for this animation
	std::vector<Bone*> mBones;

	// Array of anim data
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