#include "FrameBuffer.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexBuffer.h"
#include "../MemoryManager/AssetManager.h"

FrameBuffer::FrameBuffer(int width, int height, int subsamples) :
	mShader(nullptr),
	mVertexBuffer(nullptr),
	mAssetManager(AssetManager::Get()),
	mMSAAFrameBuffer(),
	mTextureMultiSampled(0),
	mRenderBufferMultiSampled(0),
	mFrameBuffer(0),
	mTexture(0),
	//mRenderBufferID(0),
	mBloomMaskFrameBuffer(0),
	mBloomMaskTexture(0),
	mBloomMaskRenderBuffer(0),
	mBloomBlurHorizontalFrameBuffer(0),
	mBloomBlurHorizontalTexture(0),
	mBloomBlurVerticalFrameBuffer(0),
	mBloomBlurVerticalTexture(0),
	mTextureUnit(static_cast<int>(TextureUnit::FrameBuffer))
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

	// Create MSAA framebuffer
	glGenFramebuffers(1, &mMSAAFrameBuffer);
	// bind the buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mMSAAFrameBuffer);

	// Create the multisampled texture as a color attachment
	glGenTextures(1, &mTextureMultiSampled);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTextureMultiSampled);
	// Create the image (use floating point value for HDR/tone mapping)
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, subsamples, GL_RGB16F, width, height, GL_TRUE);
	// Bind offscreen texture to framebuffer:
	// - target: the frame buffer type to target
	// - attachment: type of attachment to attach, this case color
	// - textarget: the type of texture to attatch
	// - texture: the actual texture to attach
	// - level: the mipmap level (no need in this case)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mTextureMultiSampled, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// Create multisampled renderbuffer object for depth and stencil attatchments
	glGenRenderbuffers(1, &mRenderBufferMultiSampled);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferMultiSampled);
	// Create a depth and stencil render buffer object
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, subsamples, GL_DEPTH24_STENCIL8, width, height);
	// Attach render buffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferMultiSampled);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// Check for frame buffer's completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}
	// Bind back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



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

	//// Create a renderbuffer object for depth and stencil attachment (won't be sample from these)
	//glGenRenderbuffers(1, &mRenderBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
	//// Create a depth and stencil render buffer object
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	//// Attatch render buffer object
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderBuffer);

	// Check for frame buffer's completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}
	// Bind back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// Bloom mask frame buffer (downsized by 1/2)
	glGenFramebuffers(1, &mBloomMaskFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mBloomMaskFrameBuffer);
	glGenTextures(1, &mBloomMaskTexture);
	glBindTexture(GL_TEXTURE_2D, mBloomMaskTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width / 2, height / 2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBloomMaskTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//// Create a renderbuffer object for depth and stencil attachment (downsized by 1/2)
	//glGenRenderbuffers(1, &mBloomMaskRenderBuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, mBloomMaskRenderBuffer);
	//// Create a depth and stencil render buffer object
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width / 2, height / 2);
	//// Attatch render buffer object
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mBloomMaskRenderBuffer);

	// Check for frame buffer's completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}

	// Bind back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Bloom horizontal blur frame buffer
	glGenFramebuffers(1, &mBloomBlurHorizontalFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mBloomBlurHorizontalFrameBuffer);
	glGenTextures(1, &mBloomBlurHorizontalTexture);
	glBindTexture(GL_TEXTURE_2D, mBloomBlurHorizontalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width / 4, height / 4, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBloomBlurHorizontalTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Check for frame buffer's completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}

	// Bind back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// Bloom vertical blur frame buffer
	glGenFramebuffers(1, &mBloomBlurVerticalFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mBloomBlurVerticalFrameBuffer);
	glGenTextures(1, &mBloomBlurVerticalTexture);
	glBindTexture(GL_TEXTURE_2D, mBloomBlurVerticalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width / 4, height / 4, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBloomBlurVerticalTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
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

	glDeleteFramebuffers(1, &mMSAAFrameBuffer);
	glDeleteTextures(1, &mTextureMultiSampled);
	glDeleteRenderbuffers(1, &mRenderBufferMultiSampled);

	glDeleteFramebuffers(1, &mFrameBuffer);
	glDeleteTextures(1, &mTexture);
	//glDeleteRenderbuffers(1, &mRenderBuffer);

	glDeleteFramebuffers(1, &mBloomMaskFrameBuffer);
	glDeleteTextures(1, &mBloomMaskTexture);
	glDeleteRenderbuffers(1, &mBloomMaskRenderBuffer);

	glDeleteFramebuffers(1, &mBloomBlurHorizontalFrameBuffer);
	glDeleteTextures(1, &mBloomBlurHorizontalTexture);

	glDeleteFramebuffers(1, &mBloomBlurVerticalFrameBuffer);
	glDeleteTextures(1, &mBloomBlurVerticalTexture);
}

void FrameBuffer::SetActive()
{
	// Draw scene as normal in the multisampled buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mMSAAFrameBuffer);

	// Frame buffer without multisampling
	//glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::End(int width, int height)
{
	// Blit multisampled buffers to normal color buffer of mFrameBuffer
	// Use mFrameBuffer's texture color attachment
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mMSAAFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// Bind to bloom mask frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mBloomMaskFrameBuffer);
	// change viewport to accomodate the downsampled buffer (1/2 size)
	glViewport(0, 0, width / 2, height / 2);
	Draw(width, height, mAssetManager->LoadShader("bloomMask"), mTexture);

	// Bind to bloom horizontal blur frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mBloomBlurHorizontalFrameBuffer);
	// change viewport to accomodate the downsampled buffer (1/4 size)
	glViewport(0, 0, width / 4, height / 4);
	// Blur gaussian horizontal blur
	Draw(width, height, mAssetManager->LoadShader("bloomBlurHorizontal"), mBloomMaskTexture);

	// Bind to bloom vertical blur frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mBloomBlurVerticalFrameBuffer);
	// change viewport to accomodate the downsampled buffer (1/4 size)
	glViewport(0, 0, width / 4, height / 4);
	// Blur gaussian vertical blur
	Draw(width, height, mAssetManager->LoadShader("bloomBlurVertical"), mBloomBlurHorizontalTexture);

	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Reset viewport size
	glViewport(0, 0, width, height);
	// Draw to the screen and finish with hdr and gamma correction

	Shader* shader = mAssetManager->LoadShader("hdrGamma");

	// Disable depth test so screen quad isn't discarded
	glDisable(GL_DEPTH_TEST);

	shader->SetActive();

	shader->SetInt("screenTexture", mTextureUnit);

	// Activate texture unit
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);

	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	int blurUnit = mTextureUnit + 2;

	shader->SetInt("blurTexture", blurUnit);
	glActiveTexture(GL_TEXTURE0 + blurUnit);
	glBindTexture(GL_TEXTURE_2D, mBloomBlurVerticalTexture);


	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	mVertexBuffer->Draw();

	// Enable depth test again
	glEnable(GL_DEPTH_TEST);

	//Draw(width, height, mAssetManager->LoadShader("hdrGamma"), mBloomBlurVerticalTexture);
}

void FrameBuffer::Draw(int width, int height, Shader* shader, int texture)
{
	// Disable depth test so screen quad isn't discarded
	glDisable(GL_DEPTH_TEST);

	shader->SetActive();

	shader->SetInt("screenTexture", mTextureUnit);

	// Activate texture unit
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);

	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindTexture(GL_TEXTURE_2D, texture);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	mVertexBuffer->Draw();

	// Enable depth test again
	glEnable(GL_DEPTH_TEST);
}
