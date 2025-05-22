#include "ShadowMap.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../MemoryManager/AssetManager.h"
#include "Renderer3D.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"

const unsigned int SHADOW_WIDTH = 8560;
const unsigned int SHADOW_HEIGHT = 8560;
float shadowNearPlane = 1.0f;
float shadowFarPlane = 1000.0f;

ShadowMap::ShadowMap() :
	mShadowConsts({}),
	mShader(nullptr),
	mVertexBuffer(AssetManager::Get()->GetRenderer()->GetVertexBuffer()),
	mShadowBuffer(AssetManager::Get()->GetRenderer()->CreateUniformBuffer(sizeof(glm::mat4), BufferBindingPoint::Shadow, "ShadowBuffer")),
	mShadowMapFrameBuffer(0),
	mShadowMap(0),
	mTextureUnit(static_cast<int>(TextureType::Shadow))
{
	// Create a framebuffer object
	glGenFramebuffers(1, &mShadowMapFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFrameBuffer);

	// Create a 2D texture for framebuffer's depth buffer
	glGenTextures(1, &mShadowMap);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attach the depth texture as framebuffer's depth buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);
	// Set read and draw buffer to none since this does not need a color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::~ShadowMap()
{
	std::cout << "Delete shadow map\n";

	glDeleteFramebuffers(1, &mShadowMapFrameBuffer);

	glDeleteTextures(1, &mShadowMap);
}

void ShadowMap::SetActive(float size, float near, float far, const glm::vec3& pos, const glm::vec3& target)
{
	shadowNearPlane = near;
	shadowFarPlane = far;

	// First pass will render to shadow map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Bind to frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFrameBuffer);
	// Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Create an othographic projection for a directional shadow from the light's perspective
	glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, near, far);
	// Create a look at
	glm::mat4 lightView = glm::lookAt(pos, target, glm::vec3(0.0f, 1.0f, 0.0f));

	// Multiply the new view and projection to create the light space transform matrix
	mShadowConsts.lightSpace = lightProjection * lightView;

	// Update the light shadow buffer's data with the new light space matrix
	mShadowBuffer->UpdateBufferData(&mShadowConsts);
}

void ShadowMap::DrawDebug(Shader* s)
{
	glViewport(0, 0, 400, 300);
	s->SetActive();
	s->SetFloat("nearPlane", shadowNearPlane);
	s->SetFloat("farPlane", shadowFarPlane);
	s->SetInt("depthMap", mTextureUnit);
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);

	mVertexBuffer->Draw();
}

void ShadowMap::End(int width, int height) const
{
	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set viewport back to screen's width and height
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::BindShadowMapToShader(Shader* s, const std::string& uniformName) const
{
	s->SetActive();
	s->SetInt(uniformName, mTextureUnit);
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
}
