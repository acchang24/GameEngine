#pragma once
#include <glm/glm.hpp>

struct BoneData
{
	// bone's index for the bone matrices
	int index;
	// offset matrix transforms a vertex from model space to bone space (sets model's pose)
	glm::mat4 offset;
};