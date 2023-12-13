#pragma once
#include <vector>
#include <glm/glm.hpp>

/*
	This header file contains structs that define different vertex layouts.
*/

// Enum class to store different vertex layouts types
enum class VertexLayout
{
	VertexPos; // VertexPos contains a Vector3 representing position (3 float values)
};

struct VertexPos
{
	glm::vec3 pos;
};

// Calculates the stride or spacing between consecutive vertex attributes.
// @returns - std::vector of ints that represents the stride values between each attribute.
static std::vector<int> GetVertexFormatStrides(VertexLayout layout)
{
	std::vector<int> strides;

	switch (layout)
	{
	case VertexLayout::VertexPos:
		strides.emplace_back(3);
		break;
	}
}