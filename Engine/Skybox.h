#pragma once
#include "CubeMap.h"
#include <glm/glm.hpp>

class VertexBuffer;
class Shader;

class Skybox
{
public:
	// Skybox constructor creates a cube map and its vertex buffer
	// @param - const std::vector<std::string>& for the cube map texture face names
	Skybox(const std::vector<std::string>& faceNames);
	~Skybox();

	// Draws the skybox
	// @param - const glm::mat4& for the view and proj matrix
	void Draw(const glm::mat4& view, const glm::mat4& proj);

	// Gets the skybox's shader
	// @return - Shader* for the skybox's shader
	Shader* GetShader() { return mShader; }

	// Gets the skybox's cube map
	// @return - CubeMap* for the skybox's cubemap
	CubeMap* GetCubeMap() { return mCubeMap; }

private:
	// The skybox's vertex buffer
	VertexBuffer* mVertexBuffer;

	// The skybox's shader
	Shader* mShader;

	// The skybox's cube map
	CubeMap* mCubeMap;
};