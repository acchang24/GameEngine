#pragma once
#include <SDL2/SDL.h>

class FrameBuffer;
class FrameBufferMultiSampled;
class Shader;

class Renderer3D
{
public:
	// Returns the instance of an Renderer3D
	// @return - Renderer3D* for the instance of an Renderer3D
	static Renderer3D* Get();

	// Initializes SDL, sets OpenGL attributes, creates the game window, creates the OpenGL context, and sets up GLAD.
	// Enable any OpenGL functionality at the end
	bool Init(int width, int height, int subsamples, int vsync, bool fullscreen, SDL_bool mouseCaptured,const char* title);

	// De-allocates any resources from SDL
	void Shutdown();


	void BeginFrame();
	void EndFrame();

	// Creates a frame buffer for the renderer to use
	// @param - int for the width of the frame buffer
	// @param - int for the height of the frame buffer
	// @param - int for the number of subsamples used for anti-aliasing
	// @param - bool for if the framebuffer uses multi-sample
	// @return - FrameBuffer* for the newly created framebuffer
	FrameBuffer* CreateFrameBuffer(int width, int height);

	FrameBufferMultiSampled* CreateMultiSampledFrameBuffer(int width, int height, int subsamples);

	void SetFrameBufferShader(FrameBuffer* frameBuffer, Shader* shader);

	// Static function that triggers everytime the window is resized.
	static int ResizeWindowEventWatcher(void* data, SDL_Event* event);

	// Toggles capturing the mouse in the window
	static void ToggleMouseCapture();

	// Retrieves the main frame buffer
	// @return - FrameBuffer* for the main frame buffer
	FrameBufferMultiSampled* GetMainFrameBuffer() { return mMainFrameBuffer; }
	FrameBuffer* GetBloomMaskFrameBuffer() { return mBloomMaskFrameBuffer; }
	FrameBuffer* GetBloomBlurHorizontalFrameBuffer() { return mBloomBlurHorizontalFrameBuffer; }
	FrameBuffer* GetBloomBlurVerticalFrameBuffer() { return mBloomBlurVerticalFrameBuffer; }
	FrameBufferMultiSampled* GetResizeMultiSampledFrameBuffer() { return mResizeMultiSampledFrameBuffer;  }
	FrameBuffer* GetResizeFrameBuffer() { return mResizeFrameBuffer; }

	// Gets the number of subsamples used for anti-aliasing
	// @return - int for the number of subsamples
	int GetNumSubsamples() const { return mNumSubsamples; }
	// Sets the number of subsamples used for anti-aliasing
	// @param - int for the number of subsamples
	void SetNumSubsamples(int subsamples) { mNumSubsamples = subsamples; }


private:
	Renderer3D();
	~Renderer3D();

	// Framebuffers - Add any additional framebuffers here if needed
	// and use CreateFrameBuffers() to generate a new one.
	// Add a specific getter for any new framebuffer created.
	FrameBufferMultiSampled* mMainFrameBuffer;

	FrameBuffer* mBloomMaskFrameBuffer;
	FrameBuffer* mBloomBlurHorizontalFrameBuffer;
	FrameBuffer* mBloomBlurVerticalFrameBuffer;
	FrameBufferMultiSampled* mResizeMultiSampledFrameBuffer;
	FrameBuffer* mResizeFrameBuffer;
	
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
