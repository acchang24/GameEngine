#pragma once
#include <glm/glm.hpp>

// Struct to hold data of a skeleton's bone
struct BoneData
{
	// bone's index for the bone matrices
	int index;
	// offset matrix transforms a vertex from model space to bone space (sets model's pose)
	// This is sometimes called the inverse bind pose matrix
	glm::mat4 offset;
};