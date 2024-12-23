#include "PointShadowMap.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "UniformBuffer.h"

const unsigned int SHADOW_WIDTH = 8560;
const unsigned int SHADOW_HEIGHT = 8560;
float nearPlane = 1.0f;
float farPlane = 100.0f;

PointShadowMap::PointShadowMap() :
	mPointShadowConsts({}),
	mPointShadowBuffer(new UniformBuffer(sizeof(PointShadowMapConsts), BufferBindingPoint::PointShadow, "PointShadowBuffer")),
	mPointShadowMapFrameBuffer(0),
	mPointShadowMap(0),
	mTextureUnit(static_cast<int>(TextureUnit::PointShadow))
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

void PointShadowMap::SetActive(float near, float far, const glm::vec3 pos)
{
	float aspect = static_cast<float>(SHADOW_WIDTH / SHADOW_HEIGHT);

	float nearPlane = near;
	float farPlane = far;

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	mPointShadowConsts.shadowTransforms[0] = shadowProj * glm::lookAt(pos, pos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	mPointShadowConsts.shadowTransforms[1] = shadowProj * glm::lookAt(pos, pos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	mPointShadowConsts.shadowTransforms[2] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mPointShadowConsts.shadowTransforms[3] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	mPointShadowConsts.shadowTransforms[4] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	mPointShadowConsts.shadowTransforms[5] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	// First pass will render to shadow map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Bind to frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mPointShadowMapFrameBuffer);
	// Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	mPointShadowBuffer->UpdateBufferData(&mPointShadowConsts);
}

void PointShadowMap::End(int width, int height) const
{
	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set viewport back to screen's width and height
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
