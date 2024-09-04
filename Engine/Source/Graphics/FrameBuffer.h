#pragma once
#include "Texture.h"

class Shader;
class VertexBuffer;
class AssetManager;

// Creates an OpenGL frame buffer that allows an additional target 
// to render to. Custom color, depth, and stencil buffers can be defined
// through this buffer. This allows for offscreen rendering and post processing effects.
class FrameBuffer
{
public:
	// FrameBuffer constructor
	// @param - int for the screen's width
	// @param - int for the screen's height
	// @param - int for the number of subsamples for anti aliasing
	FrameBuffer(int width, int height, int subsamples, bool multiSampled);
	~FrameBuffer();

	// Starts the frame buffer by binding this frame buffer object
	// as the current target. This is called right before clearing buffers
	// in the main render loop. Any renders should be done as normal after
	// this call.
	void SetActive() const;
	// Binds the frame buffer to the default one, then
	// disables the depth test, clears any relevant buffers,
	// draws the frame buffer, and enable depth test back again.
	// This is called at the end of the render loop, before swapping
	// buffers.
	// @param - int for the screen's width
	// @param - int for the screen's height
	void End(int width, int height);

	// Draws the frame buffer's off screen quad
	void Draw(int width, int height, Shader* shader, int texture);

	// Gets the frame buffer's shader
	// @return - Shader* for the frame buffer's shader
	Shader* GetShader() { return mShader; }

	// Sets the frame buffer's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* s) { mShader = s; }

private:
	// The shader used to help draw the frame buffer's quad
	Shader* mShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	// Pointer to the assetmanager
	AssetManager* mAssetManager;

	// MSAA framebuffer
	unsigned int mMSAAFrameBuffer;
	// The multisampled offscreen color attachment texture 
	unsigned int mTextureMultiSampled;
	// The multisampled render buffer for depth and stencil attachments
	unsigned int mRenderBufferMultiSampled;

	// Frame buffer object as a reference id
	unsigned int mFrameBuffer;
	// The offscreen color attachment texture created after blitting the multisample texture
	unsigned int mTexture;
	// Render buffer for depth and stencil attatchments as a reference
	unsigned int mRenderBuffer;

	//// Frame buffer used for bloom mask
	//unsigned int mBloomMaskFrameBuffer;
	//// Bloom mask texture downsized by 1/2 that masks off dark spots and shows only bright areas
	//unsigned int mBloomMaskTexture;
	//// Bloom mask render buffer
	//unsigned int mBloomMaskRenderBuffer;

	//unsigned int mBloomBlurHorizontalFrameBuffer;
	//unsigned int mBloomBlurHorizontalTexture;

	//unsigned int mBloomBlurVerticalFrameBuffer;
	//unsigned int mBloomBlurVerticalTexture;

	// Int for the frame buffer's texture unit, this is initialized to 11 in the initializer list
	int mTextureUnit;
};
