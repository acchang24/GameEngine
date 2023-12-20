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
	VertexPosPhong, // VertexPosPhong contains position and normal
	VertexColor, // VertexColor contains position and color
	VertexColorPhong, // VertexColorPhong contains position, normal, and color
	VertexTexture, // VertexTexture contains position, normal, and texture(uv) coordinates
	VertexColorTexture, // VertexColorTexture contains position, color, and texture(uv) coordinates
};

struct VertexPos
{
	glm::vec3 pos;
};

struct VertexPosPhong
{
	glm::vec3 pos;
	glm::vec3 normal;
};

struct VertexColor
{
	glm::vec3 pos;
	glm::vec4 color;
};

struct VertexColorPhong
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec4 color;
};

struct VertexTexture
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct VertexColorTexture
{
	glm::vec3 pos;
	glm::vec4 color;
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
		strides = {3};
		break;
	case VertexLayout::VertexPosPhong:
		strides = { 3, 3 };
		break;
	case VertexLayout::VertexColor:
		strides = { 3, 4 };
		break;
	case VertexLayout::VertexColorPhong:
		strides = { 3, 3, 4 };
		break;
	case VertexLayout::VertexTexture:
		strides = { 3, 3, 2 };
		break;
	case VertexLayout::VertexColorTexture:
		strides = { 3, 4, 2 };
		break;
	}

	return strides;
}
