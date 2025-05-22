#pragma once
#include "Texture.h"

class Renderer3D;
class Shader;
class VertexBuffer;

// Creates an OpenGL frame buffer that allows an additional target 
// to render to. Custom color, depth, and stencil buffers can be defined
// through this buffer. This allows for offscreen rendering and post processing effects.
class FrameBuffer
{
public:
	// FrameBuffer constructor
	// @param - int for the screen/window's width
	// @param - int for the screen/window's height
	// @param - Renderer3D* for renderer 
	// @param - Shader* for the framebuffer's shader
	FrameBuffer(int width, int height, Renderer3D* renderer, Shader* shader);
	virtual ~FrameBuffer();

	// Generates a new frame buffer
	// @param - int for the screen/window's width
	// @param - int for the screen/window's height
	void Load(int width, int height);

	// Deletes the framebuffer, texture, and renderbuffer references
	virtual void Unload();

	// Binding to this frame buffer object, clears its color and depth buffers, and readjusts the viewport
	// so that it matches the frame buffer's dimensions. It then sets the shader to active
	virtual void SetActive();

	// Binds this framebuffer then adjusts the viewport so it matches the framebuffer's size.
	// It then disables depth test, sets the shader to active, and draws to the framebuffer using 
	// the specified texture.
	void Draw(unsigned int texture);

	// Gets the frame buffer's shader
	// @return - Shader* for the frame buffer's shader
	Shader* GetShader() { return mShader; }

	// Sets the frame buffer's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* s) { mShader = s; }

	// Returns the offscreen color attachment texture
	// @return - unsigned int for the id of the color attachment texture
	unsigned int GetTexture() const { return mTexture; }

	// Gets the width ratio relative to original screen size
	// @return - float for the size
	float GetWidthRatio() const { return mWidthRatio; }

	// Gets the ratio ratio relative to original screen size
	// @return - float for the size
	float GetHeightRatio() const { return mHeightRatio; }

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

	// Frame buffer's width size relative to screen
	float mWidthRatio;

	// Frame buffer's height size relative to screen
	float mHeightRatio;
};
