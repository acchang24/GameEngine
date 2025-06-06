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
	// @param - Renderer3D* for renderer 
	// @param - Shader* for the framebuffer's shader
	FrameBufferMultiSampled(int width, int height, int subsamples, Renderer* renderer, Shader* shader);
	~FrameBufferMultiSampled();

	// Generates a new multi-sampled framebuffer
	// @param - int for the screen/window's width
	// @param - int for the screen/window's height
	// @param - int for the number of subsamples used for anti-aliasing
	void Load(int width, int height, int subsamples);

	// Deletes the framebuffer, texture, and renderbuffer references as well as its parent's references
	void Unload() override;

	// Binding to this frame buffer object, clears its color and depth buffers, and readjusts the viewport
	// so that it matches the frame buffer's dimensions. It then sets the shader to active
	void SetActive() override;

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
