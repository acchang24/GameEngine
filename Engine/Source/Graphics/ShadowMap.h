#pragma once
#include <glm/glm.hpp>

class Shader;
class VertexBuffer;

class ShadowMap
{
public:
	ShadowMap(const glm::vec3& pos);
	~ShadowMap();

	void SetActive();

	void DrawDebug(Shader* s);

	void End(int width, int height, Shader* s);

	// Gets the frame buffer's shader
	// @return - Shader* for the frame buffer's shader
	Shader* GetShader() { return mShader; }

	// Sets the frame buffer's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* s) { mShader = s; }

private:
	glm::mat4 mLightSpace;

	glm::vec3 mLightPos;

	// The shader used to help draw the frame buffer's quad
	Shader* mShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	unsigned int mShadowMapFrameBuffer;

	unsigned int mShadowMap;

};