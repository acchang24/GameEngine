#pragma once
#include <glm/glm.hpp>

// Max number of bones a skelton can have
constexpr unsigned int MAX_BONES = 100;

// Struct for skeleton constants that are going to be sent to a uniform buffer in the shaders
struct SkeletonConsts
{
	glm::mat4 finalBoneMatrices[MAX_BONES];
};