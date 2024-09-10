#include "FrameBufferMultiSampled.h"
#include <iostream>
#include <glad/glad.h>
#include "Shader.h"
#include "VertexBuffer.h"

FrameBufferMultiSampled::FrameBufferMultiSampled(int windowWidth, int windowHeight, int subsamples, float size) :
	FrameBuffer(windowWidth, windowHeight, size),
	mMSAAFrameBuffer(0),
	mTextureMultiSampled(0),
	mRenderBufferMultiSampled(0)
{
	Load(windowWidth, windowHeight, subsamples);
}

FrameBufferMultiSampled::~FrameBufferMultiSampled()
{
	std::cout << "Delete framebuffer multisampled" << std::endl;
	
	Unload();
}

void FrameBufferMultiSampled::Load(int windowWidth, int windowHeight, int subsamples)
{
	// Re-adjust the width/height of the framebuffer to the new window width/height
	// and based of frame buffer size (this is in case of a window resize)
	mWidth = windowWidth * mSize;
	mHeight = windowHeight * mSize;

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

void FrameBufferMultiSampled::SetActive() const
{
	// Draw scene in the multisampled buffer
	glBindFramebuffer(GL_FRAMEBUFFER, mMSAAFrameBuffer);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBufferMultiSampled::Draw(unsigned int texture)
{
	// Bind back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Reset viewport size
	glViewport(0, 0, mWidth, mHeight);

	// Disable depth test so screen quad isn't discarded
	glDisable(GL_DEPTH_TEST);

	mShader->SetActive();

	// Bind the normal texture (everything that is drawn to the screen)
	mShader->SetInt("screenTexture", mTextureUnit);
	glActiveTexture(GL_TEXTURE0 + mTextureUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	// Bind the blur texture (texture used for post-processing)
	int blurUnit = mTextureUnit + 3;
	mShader->SetInt("blurTexture", blurUnit);
	glActiveTexture(GL_TEXTURE0 + blurUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	mVertexBuffer->Draw();

	// Enable depth test again
	glEnable(GL_DEPTH_TEST);
}

void FrameBufferMultiSampled::BlitBuffers()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mMSAAFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
