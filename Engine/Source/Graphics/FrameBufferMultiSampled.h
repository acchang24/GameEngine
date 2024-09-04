#pragma once
#include "FrameBuffer.h"

class FrameBufferMultiSampled : public FrameBuffer
{
public:
	FrameBufferMultiSampled(int width, int height, int subsamples);
	~FrameBufferMultiSampled();

	void SetActive() const override;

	void End(unsigned int texture) override;

	void BlitBuffers();

private:
	// MSAA framebuffer
	unsigned int mMSAAFrameBuffer;
	// The multisampled offscreen color attachment texture 
	unsigned int mTextureMultiSampled;
	// The multisampled render buffer for depth and stencil attachments
	unsigned int mRenderBufferMultiSampled;
};