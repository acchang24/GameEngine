#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

/*
	This header file contains structs that define different vertex layouts.
*/

// Enum class to store different vertex layouts types
// Each value of this enum has a corresponding struct that
// defines the vertex values.
enum class VertexLayout
{
	Vertex2D,			// Vertex2D contains 2d position and textures
	VertexPos,			// VertexPos position
	VertexColor,		// VertexColor contains position and color
	VertexTexture,		// VertexTexture contains position and texture(uv)
	Vertex,				// Vertex contains position, normal, texture(uv), tangent, bitangent
	VertexAnim,			// Vertex contains position, normal, texture(uv), tangent, bitangent, bone ids, and bone weights
	VertexSimple,		// Vertex contains position, normal, and texture(uv) coordinates
	VertexScreenQuad	// VertexScreenQuad contains position and texutre coordinates (used for frame buffers)
};

const int MAX_BONE_INFLUENCE = 4;

struct Vertex2D
{
	glm::vec2 pos;
	glm::vec2 uv;
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

struct VertexTexture
{
	glm::vec3 pos;
	glm::vec2 uv;
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};


struct VertexAnim
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	int boneIDs[MAX_BONE_INFLUENCE] = {-1, -1, -1, -1};
	float weights[MAX_BONE_INFLUENCE];
};

struct VertexSimple
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


// Set Vertex2D attribute pointers
static int Vertex2DAttribPointers()
{
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)(0));
	// vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, uv));

	return 2;
}

// Set VertexPos attribute pointers
static int VertexPosAttribPointer()
{
	// Enable each attribute
	glEnableVertexAttribArray(0);
	//   Set vertex attributes pointers
	//   Link Vertex Attributes with glVertexAttribPointer():
	// - First argument specifies which vertex attribute to configure. This attribute is specified within the vertex shader
	// - Second argument specifies the size or number of values for the vertex attribute.
	// - Third argument specifies the type of the data, which in this case is a GL_Float (vec* in GLSL)
	// - Fourth argument specifies if the data is going to be normalized.
	// - Fifth argument is the stride, and defines the space between consecutive vertex attributes
	// - Last argument is type void*, and is the offset of where the position data begins in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPos), (void*)(0));

	return 1;
}

// Set VertexColor attribute pointers
static int VertexColorAttribPointer()
{
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)(0));
	// vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)offsetof(VertexColor, color));

	return 2;
}

static int VertexTextureAttribPointer()
{
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), (void*)(0));
	// vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), (void*)offsetof(VertexTexture, uv));

	return 2;
}

// Set Vertex attribute pointers
static int VertexAttribPointer()
{
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	// vertex normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	return 5;
}

// Set VertexAnim attribute pointers
static int VertexAnimAttribPointer()
{
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)(0));
	// vertex normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, normal));
	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, uv));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, bitangent));
	// vertex bone ids
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(VertexAnim), (void*)offsetof(VertexAnim, boneIDs));
	// bone weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, weights));

	return 7;
}

// Set VertexSimple attribute pointers
static int VertexSimpleAttribPointer()
{
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSimple), (void*)(0));
	// vertex normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexSimple), (void*)offsetof(VertexSimple, normal));
	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexSimple), (void*)offsetof(VertexSimple, uv));

	return 3;
}

// Set VertexScreenQuad attribute pointers
static int VertexScreenQuadAttribPointer()
{
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexScreenQuad), (void*)(0));
	// vertex texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexScreenQuad), (void*)offsetof(VertexScreenQuad, uv));

	return 3;
}
