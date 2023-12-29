#include "FrameBuffer.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexBuffer.h"

FrameBuffer::FrameBuffer(int width, int height) :
	mShader(nullptr),
	mVertexBuffer(nullptr),
	mFrameBufferID(0),
	mTextureID(0),
	mRenderBufferID(0),
	mTextureUnit(static_cast<int>(TextureUnit::FrameBuffer)),
	mIsActive(false)
{
	// Vertex attributes for screen quad that fills the entire screen in Normalized Device Coordinates
	VertexScreenQuad quadVertices[] = {
		// positions				// texCoords
		glm::vec2(-1.0f,  1.0f),  glm::vec2(0.0f, 1.0f),
		glm::vec2(-1.0f, -1.0f),  glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, -1.0f),  glm::vec2(1.0f, 0.0f),

		glm::vec2(-1.0f,  1.0f),  glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, -1.0f),  glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f,  1.0f),  glm::vec2(1.0f, 1.0f)
	};
	mVertexBuffer = new VertexBuffer(quadVertices, 0, sizeof(quadVertices), 0, sizeof(quadVertices) / sizeof(VertexScreenQuad), 0, VertexLayout::VertexScreenQuad);

	// Create frame buffer object
	glGenFramebuffers(1, &mFrameBufferID);
	// bind the frame buffer object
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

	// Create texture object
	glGenTextures(1, &mTextureID);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	// Create the image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Bind offscreen texture to framebuffer:
	// - target: the frame buffer type to target
	// - attachment: type of attachment to attach, this case color
	// - textarget: the type of texture to attatch
	// - texture: the actual texture to attach
	// - level: the mipmap level (no need in this case)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);

	// Create a renderbuffer object for depth and stencil attachment (won't be sample from these)
	glGenRenderbuffers(1, &mRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
	// Create a depth and stencil render buffer object
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	// Attatch render buffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);

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

	glDeleteFramebuffers(1, &mFrameBufferID);

	glDeleteTextures(1, &mTextureID);

	glDeleteRenderbuffers(1, &mRenderBufferID);
}

void FrameBuffer::SetActive()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
}

void FrameBuffer::End(int width, int height)
{
	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Disable depth test so screen quad isn't discarded
	glDisable(GL_DEPTH_TEST);
	// Specify color to clear the screen
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Clear color buffers and any other relevant buffers again
	glClear(GL_COLOR_BUFFER_BIT);
	// Draw the frame buffer
	Draw(width, height);
	// Enable depth test again
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::Draw(int width, int height)
{
	mShader->SetActive();

	// Activate texture unit
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	mShader->SetInt("screenTexture", mTextureUnit);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glActiveTexture(GL_TEXTURE0);
	mVertexBuffer->Draw();
}
