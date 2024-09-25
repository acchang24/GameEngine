#pragma once
#include <glm/glm.hpp>

// Struct for shadow map data to be sent to the shaders
struct ShadowMapConsts
{
	glm::mat4 lightSpace; // matrix for transforming world-space vectors into space that's visible from the light's point of view (light-space)
};

class Shader;
class UniformBuffer;
class VertexBuffer;

// ShadowMap is used to create shadows that are cast from a directional light.
// This creates a depth map using a framebuffer's depth buffer. To render
// a shadow, render a first pass in Game::Render() after setting ShadowMap::SetActive(). This
// will render to the depth/shadow map, which will be used to calculate whether the fragments 
// are in shadow in a second normal render pass.
class ShadowMap
{
public:
	// ShadowMap constructor:
	// Creates a frame buffer 
	ShadowMap();
	~ShadowMap();

	// Sets the viewport to fit the depth map's size and binds the framebuffer to
	// to draw to this shadow map's frame buffer. It will clear the framebuffer's depth buffer
	// and calculates the light space matrix. It then sends the light space matrix to the mShadowBuffer
	void SetActive(float size, float near, float far, const glm::vec3& pos, const glm::vec3& target);

	// Renders the shadow/depth map for debug purposes
	void DrawDebug(Shader* s);

	// Binds back to the default frame buffer, resets the viewport back to the original size, and clears the color/depth buffers
	// @param - int for the width of the viewport
	// @param - int for the height of the viewport
	void End(int width, int height) const;

	// Activates the texture unit to match the shadow map and binds the depth map as the texture within a shader
	// @param - Shader* to activate
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

	// Uniform buffer to send shadow data to shaders
	UniformBuffer* mShadowBuffer;

	// Frame buffer for the shadow map
	unsigned int mShadowMapFrameBuffer;

	// Texture used for the depth map
	unsigned int mShadowMap;

	// Shadow texture unit
	int mTextureUnit;
};
