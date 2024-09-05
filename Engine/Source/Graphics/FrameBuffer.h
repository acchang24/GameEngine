#pragma once
#include "Texture.h"

class Shader;
class VertexBuffer;

// Creates an OpenGL frame buffer that allows an additional target 
// to render to. Custom color, depth, and stencil buffers can be defined
// through this buffer. This allows for offscreen rendering and post processing effects.
class FrameBuffer
{
public:
	// FrameBuffer constructor
	// @param - int for the screen's width
	// @param - int for the screen's height
	FrameBuffer(int width, int height);
	virtual ~FrameBuffer();

	// Generates a new frame buffer
	// @param - int for the framebuffer's width
	// @param - int for the framebuffer's height
	void Load(int width, int height);
	// Deletes the framebuffer, texture, and renderbuffer references
	virtual void Unload();

	// Starts the frame buffer by binding this frame buffer object
	// as the current target. This is called right before clearing buffers
	// in the main render loop. Any renders should be done as normal after
	// this call.
	virtual void SetActive() const;

	// Binds this frame then adjusts the viewport so it matches this framebuffer's size.
	// It then disables depth test, sets the shader to active, and draws to the framebuffer.
	virtual void End(unsigned int texture);

	// Draws the frame buffer's off screen quad
	void Draw(int texture);

	// Gets the frame buffer's shader
	// @return - Shader* for the frame buffer's shader
	Shader* GetShader() { return mShader; }

	// Sets the frame buffer's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* s) { mShader = s; }

	// Returns the offscreen color attachment texture
	// @return - unsigned int for the id of the color attachment texture
	unsigned int GetTexture() const { return mTexture; }

protected:
	// The shader used to help draw the frame buffer's quad
	Shader* mShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	// Frame buffer object as a reference id
	unsigned int mFrameBuffer;
	// The offscreen color attachment texture created after blitting the multisample texture
	unsigned int mTexture;
	// Render buffer for depth and stencil attatchments as a reference
	unsigned int mRenderBuffer;

	// Int for the frame buffer's texture unit, this is initialized to 11 in the initializer list
	int mTextureUnit;

	// Framebuffer's width
	int mWidth;

	// Framebuffer's height
	int mHeight;
};
