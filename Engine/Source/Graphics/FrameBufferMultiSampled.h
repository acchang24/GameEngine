#pragma once
#include "FrameBuffer.h"

// Child of FrameBuffer class. Used to draw to a frame buffer that requires multisampling.
// If anti-aliasing is being used, this framebuffer should be the first/main offscreen frame buffer
// that is drawn to. Ending the multisampled framebuffer will also draw to the default frame buffer.
class FrameBufferMultiSampled : public FrameBuffer
{
public:
	// FrameBufferMultiSampled constructor
	// @param - int for the screen/window's width
	// @param - int for the screen/window's height
	// @param - int for the number of subsamples used for anti-aliasing
	// @param - float for the frame buffer's size
	FrameBufferMultiSampled(int windowWidth, int windowHeight, int subsamples, float size);
	~FrameBufferMultiSampled();

	// Generates a new multi-sampled framebuffer
	// @param - int for the screen/window's width
	// @param - int for the screen/window's height
	// @param - int for the number of subsamples used for anti-aliasing
	void Load(int windowWidth, int windowHeight, int subsamples);
	// Deletes the framebuffer, texture, and renderbuffer references as well as its parent's references
	void Unload() override;

	// Draws the scene in the multisampled frame buffer
	void SetActive() const override;

	// Binds back to the default frame buffer, resets the viewport, and additively blends the
	// two textures together and draws on the default frame buffer
	void Draw(unsigned int texture) override;

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
