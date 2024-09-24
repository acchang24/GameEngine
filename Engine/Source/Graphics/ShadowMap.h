#pragma once
#include <glm/glm.hpp>

// Struct for shadow map data to be sent to the shaders
struct ShadowMapConsts
{
	glm::mat4 lightSpace; // light space matrix for drawing scene from the light's point of view
};

class Shader;
class UniformBuffer;
class VertexBuffer;

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void SetActive(float size, float near, float far, const glm::vec3& pos, const glm::vec3& target);

	void DrawDebug(Shader* s);

	void End(int width, int height) const;

	void BindShadowMapToShader(Shader* s) const;

	// Gets the frame buffer's shader
	// @return - Shader* for the frame buffer's shader
	Shader* GetShader() { return mShader; }

	// Sets the frame buffer's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* s) { mShader = s; }

	UniformBuffer* GetShadowBuffer() { return mShadowBuffer; }

private:
	// Shadow constants
	ShadowMapConsts mShadowConsts;

	// The shader used to help draw the frame buffer's quad
	Shader* mShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	UniformBuffer* mShadowBuffer;

	int mTextureUnit;

	// Frame buffer for the shadow map
	unsigned int mShadowMapFrameBuffer;

	// Texture used for the depth map
	unsigned int mShadowMap;
};
