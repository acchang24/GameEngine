#pragma once
#include <glm/glm.hpp>

struct PointShadowMapConsts
{
	glm::mat4 shadowTransforms[6];
};

class UniformBuffer;

class PointShadowMap
{
public:
	PointShadowMap();
	~PointShadowMap();

	void SetActive(float near, float far, const glm::vec3 pos);

	void End(int width, int height) const;

private:
	PointShadowMapConsts mPointShadowConsts;

	UniformBuffer* mPointShadowBuffer;

	// Frame buffer for point shadow map
	unsigned int mPointShadowMapFrameBuffer;

	// Cube map used for point shadows
	unsigned int mPointShadowMap;

	// PointShadow texture unit
	int mTextureUnit;
};
