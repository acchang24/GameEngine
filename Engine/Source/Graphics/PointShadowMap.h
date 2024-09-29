#pragma once

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
};
