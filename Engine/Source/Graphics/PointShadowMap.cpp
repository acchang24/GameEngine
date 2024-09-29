#include "PointShadowMap.h"
#include <iostream>
#include <glad/glad.h>

const unsigned int SHADOW_WIDTH = 8560;
const unsigned int SHADOW_HEIGHT = 8560;

PointShadowMap::PointShadowMap() :
	mPointShadowMapFrameBuffer(0),
	mPointShadowMap(0)
{
	glGenTextures(1, &mPointShadowMap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, mPointShadowMap);

	// Assign each side of cubemap a 2D depth-valued texture image
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	}
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, mPointShadowMapFrameBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mPointShadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PointShadowMap::~PointShadowMap()
{
	std::cout << "Deleted point shadow map\n";

	glDeleteFramebuffers(1, &mPointShadowMapFrameBuffer);

	glDeleteTextures(1, &mPointShadowMap);
}
