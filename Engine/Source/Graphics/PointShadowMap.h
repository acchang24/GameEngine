#pragma once
#include <glm/glm.hpp>

struct PointShadowMapConsts
{
	glm::mat4 shadowTransforms[6];
};

class PointShadowMap
{
public:
	PointShadowMap();
	~PointShadowMap();

private:
	// Frame buffer for point shadow map
	unsigned int mPointShadowMapFrameBuffer;

	// Cube map used for point shadows
	unsigned int mPointShadowMap;

	// PointShadow texture unit
	int mTextureUnit;
};
