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
	VertexPos, // VertexPos just contains vec3 position
	VertexColor, // VertexColor contains position and color
	Vertex, // Vertex contains position, normal, and texture(uv) coordinates
	VertexScreenQuad // VertexScreenQuad contains position and texutre coordinates (used for frame buffers)
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

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct VertexScreenQuad
{
	glm::vec2 pos;
	glm::vec2 uv;
};

// Calculates the stride or spacing between consecutive vertex attributes.
// @returns - std::vector of ints that represents the stride values between each attribute.
static std::vector<int> GetVertexLayoutStrides(VertexLayout layout)
{
	std::vector<int> strides;

	switch (layout)
	{
	case VertexLayout::VertexPos:
		strides = { 3 };
		break;
	case VertexLayout::VertexColor:
		strides = { 3, 4 };
		break;
	case VertexLayout::Vertex:
		strides = { 3, 3, 2 };
		break;
	case VertexLayout::VertexScreenQuad:
		strides = { 2, 2 };
		break;
	}

	return strides;
}
