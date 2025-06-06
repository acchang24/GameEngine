#include "FrameBufferMultiSampled.h"
#include <iostream>
#include <glad/glad.h>
#include "Shader.h"
#include "VertexBuffer.h"

FrameBufferMultiSampled::FrameBufferMultiSampled(int width, int height, int subsamples, Renderer* renderer, Shader* shader) :
	FrameBuffer(width, height, renderer, shader),
	mMSAAFrameBuffer(0),
	mTextureMultiSampled(0),
	mRenderBufferMultiSampled(0)
{
	Load(width, height, subsamples);
}

FrameBufferMultiSampled::~FrameBufferMultiSampled()
{
	std::cout << "Delete framebuffer multisampled" << std::endl;
	
	Unload();
}

void FrameBufferMultiSampled::Load(int width, int height, int subsamples)
{
	// Re-adjust the width/height of the framebuffer to the new window width/height
	// and based of frame buffer size (this is in case of a window resize)
	mWidth = width;
	mHeight = height;

	if (mFrameBuffer == 0 && mTexture == 0 && mRenderBuffer == 0)
	{
		FrameBuffer::Load(mWidth, mHeight);
	}

	// Create MSAA framebuffer
	glGenFramebuffers(1, &mMSAAFrameBuffer);
	// bind the buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mMSAAFrameBuffer);

	// Create the multisampled texture as a color attachment
	glGenTextures(1, &mTextureMultiSampled);
	// bind the texture
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mTextureMultiSampled);
	// Create the image (use floating point value for HDR/tone mapping)
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, subsamples, GL_RGB16F, mWidth, mHeight, GL_TRUE);
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
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, subsamples, GL_DEPTH24_STENCIL8, mWidth, mHeight);
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
}

void FrameBufferMultiSampled::Unload()
{
	FrameBuffer::Unload();

	glDeleteFramebuffers(1, &mMSAAFrameBuffer);
	glDeleteTextures(1, &mTextureMultiSampled);
	glDeleteRenderbuffers(1, &mRenderBufferMultiSampled);

	mMSAAFrameBuffer = 0;
	mTextureMultiSampled = 0;
	mRenderBufferMultiSampled = 0;
}

void FrameBufferMultiSampled::SetActive()
{
	// Draw scene in the multisampled buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mMSAAFrameBuffer);

	// Clear the color buffer, depth buffer for this frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset viewport size to this frame buffer's dimensions
	glViewport(0, 0, mWidth, mHeight);

	// Set shader active
	mShader->SetActive();
}

void FrameBufferMultiSampled::BlitBuffers()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mMSAAFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
