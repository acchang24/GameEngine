#pragma once
#include <vector>
#include <glm/glm.hpp>

/*
	This header file contains structs that define different vertex layouts.
*/

// Enum class to store different vertex layouts types
// Each value of this enum has a corresponding struct that
// defines the vertex values.
enum class VertexLayout
{
	VertexPos, // VertexPos contains just position
	VertexColor, // VertexColor contains position and color
};

struct VertexPos
{
	glm::vec3 pos;
};

struct VertexColor
{
	glm::vec3 pos;
	glm::vec4 color;
};

// Calculates the stride or spacing between consecutive vertex attributes.
// @returns - std::vector of ints that represents the stride values between each attribute.
static std::vector<int> GetVertexLayoutStrides(VertexLayout layout)
{
	std::vector<int> strides;

	switch (layout)
	{
	case VertexLayout::VertexPos:
		strides.emplace_back(3);
		break;
	case VertexLayout::VertexColor:
		strides.emplace_back(3);
		strides.emplace_back(4);
	}

	return strides;
}
