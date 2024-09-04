#pragma once
#include "FrameBuffer.h"

class FrameBufferMultiSampled : public FrameBuffer
{
public:
	FrameBufferMultiSampled(int width, int height, int subsamples);
	~FrameBufferMultiSampled();

	void SetActive() const override;

	// Binds back to the default frame buffer, resets the viewport, and additively blends the
	// two textures together and draws on the default frame buffer
	void End(unsigned int texture) override;

	// Blits multisampled buffers to multisampled frame buffer's normal color buffer/framebuffer (mFrameBuffer)
	// Use mFrameBuffer's texture color attachment
	void BlitBuffers();

private:
	// MSAA framebuffer
	unsigned int mMSAAFrameBuffer;
	// The multisampled offscreen color attachment texture 
	unsigned int mTextureMultiSampled;
	// The multisampled render buffer for depth and stencil attachments
	unsigned int mRenderBufferMultiSampled;
};
