#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>
#include "UniformBuffer.h"

class FrameBuffer;
class FrameBufferMultiSampled;
class Shader;
class VertexBuffer;

class Renderer3D
{
public:
	// Returns an instance of an Renderer3D (singleton)
	// @return - Renderer3D* (pointer to the instance)
	static Renderer3D* Get();

	// Initializes SDL, sets OpenGL attributes, creates the game window, creates the OpenGL context, and sets up GLAD.
	// Enables any OpenGL functionality at the end
	// @param - int for the window width
	// @param - int for the window height
	// @param - int for the number of subsamples used for anti-aliasing
	// @param - int for if the renderer uses vsync
	// @param - bool for if the screen is full screen
	// @param - SDL_bool for if the mouse is captured by the window
	// @param - const char* for the game window title
	// @return - true if the renderer was successfully initialized, false if not
	bool Init(int width, int height, int subsamples, int vsync, bool fullscreen, SDL_bool mouseCaptured, const char* title);

	// De-allocates any resources from SDL, and deletes any uniform buffers and frame bufers
	void Shutdown();

	// Clears the screen, and clears the color and depth buffer bits
	void ClearBuffers();

	// Sets back to the default frame buffer, clears its color/depth buffers and resets the viewport
	void SetDefaultFrameBuffer() const;

	// Swap the buffers and present to the screen
	void EndFrame();

	// Creates a uniform buffer and saves it into the renderer's map of uniform buffers
	// @param - size_t for the buffer's size, or the amount of memory to allocate to the buffer
	// @param - BufferBindingPoint for the buffer's binding point
	// @param - const char* for the buffer's name
	// @return - UniformBuffer* for the newly created buffer object
	UniformBuffer* CreateUniformBuffer(size_t bufferSize, BufferBindingPoint bindingPoint, const char* bufferName);

	// Retrieves a uniform buffer from the renderer's uniform buffer map by name
	// @param - const std::string& for the uniform buffer name
	// @return - UniformBuffer* for the desired uniform buffer
	static UniformBuffer* GetUniformBuffer(const std::string& bufferName);

	// Creates a frame buffer for the renderer to use
	// @param - float for the frame buffer's scale relative to the window/screen size 
	// (add a float if a larger/smaller frame buffer size is needed, otherwise it will default to 1.0f)
	static FrameBuffer* CreateFrameBuffer(float scale = 1.0f);

	// Creates a multisampled frame buffer for the renderer to use
	// @param - int for the number of subsamples used for anti-aliasing
	// @param - float for the frame buffer's scale relative to the window/screen size 
	// (add a float if a larger/smaller frame buffer size is needed, otherwise it will default to 1.0f)
	static FrameBufferMultiSampled* CreateMultiSampledFrameBuffer(int subsamples, float scale = 1.0f);

	// Sets up a shader so that two textures can be additively blended together
	// @param - Shader* to set active
	// @param - unsigned int for a reference to the first texture
	// @param - unsigned int for a reference to the second texture
	// @param - int for the texture unit to activate
	void CreateBlend(Shader* shader, unsigned int texture1, unsigned int texture2, int textureUnit);

	// Gets the window's width
	// @return - int for the width
	int GetWidth() const { return mWindowWidth; }

	// Gets the window's height
	// @return - int for the height
	int GetHeight() const { return mWindowHeight; }

	// Resizes all the frame buffers to the new dimensions
	void ResizeFrameBuffers();

	// Gets the vertex buffer to draw a screen quad
	// @return - VertexBuffer* for the vertex buffer
	VertexBuffer* GetVertexBuffer() { return mVertexBuffer; }

	// Gets the number of subsamples used for anti-aliasing
	// @return - int for the number of subsamples
	int GetNumSubsamples() const { return mNumSubsamples; }

	// Sets the number of subsamples used for anti-aliasing
	// @param - int for the number of subsamples
	void SetNumSubsamples(int subsamples) { mNumSubsamples = subsamples; }

	// Sets the window's width
	// @param - int for the new width
	void SetWidth(int width) { mWindowWidth = width; }

	// Sets the window's height
	// @param - int for the new height
	void SetHeight(int height) { mWindowHeight = height; }

private:
	// FUNCTIONS
	Renderer3D();
	~Renderer3D();

	// Inititialize SDL for video and audio, and check if successful
	// @return - true if SDL was loaded successfully
	bool InitSDL() const;

	// Load default OpenGL library, and sets any OpenGL attributes
	void LoadOpenGL() const;

	// Creates a window based on if the user wants fullscreen or windowed
	// @return - true if the window was successfully created
	bool CreateWindow();

	// Creates the OpenGL context using a window
	// @return - true if the OpenGL context was successfully created
	bool CreateContext();

	// Obtain API function pointers for OpenGL/Initialize GLAD
	// and prints OpenGL info to the console
	void LoadGLAD() const;

	// Loads any additional user based SDL settings
	// @param - SDL_bool for if the mouse is captured
	void LoadSdlSettings(SDL_bool mouseCaptured) const;

	// Enable/disable any opengl capabilities
	void SetOpenGLCapabilities() const;


	// MEMBER VARIABLES
	// Map of uniform buffers
	std::unordered_map<std::string, UniformBuffer*> mUniformBuffers;

	// Vector of frame buffers used by the renderer
	std::vector<FrameBuffer*> mFrameBuffers;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

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
	
	// Window width
	int mWindowWidth;

	// Window height
	int mWindowHeight;

	// Bool for if the renderer is in fullscreen mode
	bool mIsFullScreen;
};
