#pragma once
#include <vector>
#include <SDL2/SDL.h>

class FrameBuffer;
class FrameBufferMultiSampled;
class Shader;
class UniformBuffer;
class VertexBuffer;

class Renderer3D
{
public:
	// Returns an instance of an Renderer3D (singleton)
	// @return - Renderer3D* (pointer to the instance)
	static Renderer3D* Get();

	// Initializes SDL, sets OpenGL attributes, creates the game window, creates the OpenGL context, and sets up GLAD.
	// Enable any OpenGL functionality at the end
	// @param - int for the window width
	// @param - int for the window height
	// @param - int for the number of subsamples used for anti-aliasing
	// @param - int for if the renderer uses vsync
	// @param - bool for if the screen is full screen
	// @param - SDL_bool for if the mouse is captured by the window
	// @param - const char* for the game window title
	bool Init(int width, int height, int subsamples, int vsync, bool fullscreen, SDL_bool mouseCaptured, const char* title);

	// De-allocates any resources from SDL
	void Shutdown();

	// Checks for window resize. If true, then resize all the frame buffers.
	// Clears the screen, and clears the color and depth buffer bits
	void ClearBuffers();

	// Sets back to the default frame buffer, clears its color/depth buffers and resets the viewport
	void SetDefaultFrameBuffer() const;

	// Swap the buffers and present to the screen
	void EndFrame();

	// Creates a frame buffer for the renderer to use
	// @param - int for the width of the screen/window
	// @param - int for the height of the screen/window
	// @param - float for the frame buffer's size 
	// (add a float if a larger/smaller frame buffer size is needed, otherwise it will default to 1.0f)
	FrameBuffer* CreateFrameBuffer(int screenWidth, int screenHeight, float size = 1.0f);

	// Creates a multisampled frame buffer for the renderer to use
	// @param - int for the width of the screen/window
	// @param - int for the height of the screen/window
	// @param - int for the number of subsamples used for anti-aliasing
	// @param - float for the frame buffer's size 
	// (add a float if a larger/smaller frame buffer size is needed, otherwise it will default to 1.0f)
	FrameBufferMultiSampled* CreateMultiSampledFrameBuffer(int screenWidth, int screenHeight, int subsamples, float size = 1.0f);

	// Sets up a shader so that two textures can be additively blended together
	// @param - Shader* to set active
	// @param - unsigned int for a reference to the first texture
	// @param - unsigned int for a reference to the second texture
	// @param - int for the texture unit to activate
	void CreateBlend(Shader* shader, unsigned int texture1, unsigned int texture2, int textureUnit);

	// Gets the window's width
	// @return - int for the width
	static int GetWidth();

	// Gets the window's height
	// @return - int for the height
	static int GetHeight();

	// Static function that triggers everytime the window is resized.
	// This resizes the viewport as well as the camera's projection matrix
	// given the new dimensions
	static int ResizeWindowEventWatcher(void* data, SDL_Event* event);

	// Resizes all the frame buffers to the new dimensions
	void ResizeFrameBuffers();

	// Link shader's uniform block to a uniform's binding point
	// @param - UniformBuffer* for the uniform buffer
	// @param - Shader* to link the buffer
	void LinkShaderToUniformBlock(UniformBuffer* buffer, Shader* shader);

	// Gets the vertex buffer to draw a screen quad
	// @return - VertexBuffer* for the vertex buffer
	VertexBuffer* GetVertexBuffer() { return mVertexBuffer; }

	// Gets the UniformBuffer for materials
	// @return - UniformBuffer* for the material buffer
	UniformBuffer* GetMaterialBuffer() { return mMaterialBuffer; }

	// Gets the UniformBuffer for skeleton
	// @return - UniformBuffer* for the skeleton buffer
	UniformBuffer* GetSkeletonBuffer() { return mSkeletonBuffer; }

	// Gets the number of subsamples used for anti-aliasing
	// @return - int for the number of subsamples
	int GetNumSubsamples() const { return mNumSubsamples; }

	// Sets the number of subsamples used for anti-aliasing
	// @param - int for the number of subsamples
	void SetNumSubsamples(int subsamples) { mNumSubsamples = subsamples; }

private:
	Renderer3D();
	~Renderer3D();

	// Vector of frame buffers used by the renderer
	std::vector<FrameBuffer*> mFrameBuffers;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	// A uniform buffer for passing Material data
	UniformBuffer* mMaterialBuffer;

	// A uniform buffer for passing Skeleton data
	UniformBuffer* mSkeletonBuffer;

	// SDL window used for the game
	SDL_Window* mWindow;

	// OpenGL context
	SDL_GLContext mContext;

	// Title used for the window
	const char* mWindowTitle;

	// Number of subsamples for anti-aliasing
	int mNumSubsamples;

	// If the renderer uses v-sync
	int mVSync;

	// Bool for if the renderer is in fullscreen mode
	bool mIsFullScreen;
};
