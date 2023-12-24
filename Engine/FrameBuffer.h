#pragma once

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
	~FrameBuffer();

	// Starts the frame buffer by binding this frame buffer object
	// as the current target. This is called right before clearing buffers
	// in the main render loop. Any renders should be done as normal after
	// this call.
	void SetActive();
	// Binds the frame buffer to the default one, then
	// disables the depth test, clears any relevant buffers,
	// draws the frame buffer, and enable depth test back again.
	// This is called at the end of the render loop, before swapping
	// buffers.
	// @param - int for the screen's width
	// @param - int for the screen's height
	void End(int width, int height);

	// Draws the frame buffer's off screen quad
	void Draw(int width, int height);

	// Gets the frame buffer object's id
	// @return - unsigned int for the frame buffer object's id
	unsigned int GetFrameBufferID() const { return mFrameBufferID; }
	// Gets the texture id
	// @return - unsigned int for the texture's id
	unsigned int GetTextureID() const { return mTextureID; }
	// Gets the render buffer object's id
	// @return - unsigned int for the render buffer object's id
	unsigned int GetRenderBufferID() const { return mRenderBufferID; }
	// See if the frame buffer is active or not
	// @return - bool for status
	bool IsActive() const { return mIsActive; }

	// Sets the frame buffer's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* s) { mShader = s; }

private:
	// The shader used to help draw the frame buffer's quad
	Shader* mShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	// Frame buffer object as a reference id
	unsigned int mFrameBufferID;

	// The offscreen texture as a reference id
	unsigned int mTextureID;

	// Render buffer for depth and stencil attatchments as a reference
	unsigned int mRenderBufferID;

	// Bool for if this frame buffer is active
	bool mIsActive;
};
