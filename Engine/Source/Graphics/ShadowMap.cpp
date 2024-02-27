#include "ShadowMap.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "VertexBuffer.h"

const unsigned int SHADOW_WIDTH = 8560;
const unsigned int SHADOW_HEIGHT = 8560;
float shadowNearPlane = 1.0f;
float shadowFarPlane = 1000.0f;

ShadowMap::ShadowMap(const glm::vec3& pos) :
	mLightSpace(glm::mat4(1.0f)),
	mLightPos(pos),
	mShader(nullptr),
	mVertexBuffer(nullptr),
	mShadowMapFrameBuffer(0),
	mShadowMap(0)
{
	// Vertex attributes for screen quad that fills the entire screen in Normalized Device Coordinates
	VertexScreenQuad quadVertices[] = {
		glm::vec2(-1.0f,  1.0f), glm::vec2(0.0f, 1.0f),
		glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f),

		glm::vec2(-1.0f,  1.0f), glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f,  1.0f), glm::vec2(1.0f, 1.0f)
	};
	mVertexBuffer = new VertexBuffer(quadVertices, 0, sizeof(quadVertices), 0, sizeof(quadVertices) / sizeof(VertexScreenQuad), 0, VertexLayout::VertexScreenQuad);


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
	std::cout << "Delete shadow map" << std::endl;

	delete mVertexBuffer;

	glDeleteFramebuffers(1, &mShadowMapFrameBuffer);

	glDeleteTextures(1, &mShadowMap);
}

void ShadowMap::SetActive(float size, float near, float far, const glm::vec3& pos)
{
	// Render the depth of a scene to a texture (from the light's perspective)
	glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, near, far);
	glm::mat4 lightView = glm::lookAt(pos, glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f));
	
	mLightSpace = lightProjection * lightView;

	mShader->SetActive();
	mShader->SetMat4("lightSpace", mLightSpace);

	// First pass will render to shadow map
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Bind to frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFrameBuffer);
	// Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//glCullFace(GL_FRONT);
}

void ShadowMap::DrawDebug(Shader* s)
{
	glViewport(0, 0, 400, 300);
	s->SetActive();
	s->SetFloat("nearPlane", shadowNearPlane);
	s->SetFloat("farPlane", shadowFarPlane);
	s->SetInt("depthMap", 13);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);

	mVertexBuffer->Draw();
}

void ShadowMap::End(int width, int height, Shader* s)
{
	//glCullFace(GL_BACK);

	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set viewport back to screen's width and height
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	s->SetActive();
	s->SetMat4("lightSpace", mLightSpace);

	s->SetInt("shadowMap", 13);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
}
