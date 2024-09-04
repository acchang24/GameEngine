#include "FrameBuffer.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexBuffer.h"
#include "../MemoryManager/AssetManager.h"

FrameBuffer::FrameBuffer(int width, int height) :
	mShader(nullptr),
	mVertexBuffer(nullptr),
	mFrameBuffer(0),
	mTexture(0),
	mRenderBuffer(0),
	mTextureUnit(static_cast<int>(TextureUnit::FrameBuffer)),
	mWidth(width),
	mHeight(height)
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

	// Create a frame buffer object with non multisampled texture attachment
	glGenFramebuffers(1, &mFrameBuffer);
	// bind the frame buffer object
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	// Create texture to use as a color attachment
	glGenTextures(1, &mTexture);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D, mTexture);
	// Create the image (use floating point for HDR/tone mapping)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create a renderbuffer object for depth and stencil attachment (won't be sample from these)
	glGenRenderbuffers(1, &mRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
	// Create a depth and stencil render buffer object
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	// Attatch render buffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBuffer);

	// Check for frame buffer's completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}
	// Bind back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	std::cout << "Delete framebuffer" << std::endl;

	delete mVertexBuffer;

	glDeleteFramebuffers(1, &mFrameBuffer);
	glDeleteTextures(1, &mTexture);
	glDeleteRenderbuffers(1, &mRenderBuffer);
}

void FrameBuffer::SetActive() const
{
	// Bind to this frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::End(unsigned int texture)
{
	// Bind/draw to this framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	// Adjust the viewport to the framebuffer's size
	glViewport(0, 0, mWidth, mHeight);
	
	// Disable depth test so screen quad isn't discarded
	glDisable(GL_DEPTH_TEST);

	mShader->SetActive();

	mShader->SetInt("screenTexture", mTextureUnit);

	// Activate texture unit
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);

	glBindTexture(GL_TEXTURE_2D, texture);

	mVertexBuffer->Draw();

	// Enable depth test again
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::Draw(int texture)
{
	// Disable depth test so screen quad isn't discarded
	glDisable(GL_DEPTH_TEST);

	mShader->SetActive();

	mShader->SetInt("screenTexture", mTextureUnit);

	// Activate texture unit
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);

	glBindTexture(GL_TEXTURE_2D, texture);

	mVertexBuffer->Draw();

	// Enable depth test again
	glEnable(GL_DEPTH_TEST);
}
