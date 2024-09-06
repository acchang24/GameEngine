#pragma once
#include <SDL2/SDL.h>

class Renderer2D
{
public:
	// Returns the instance of an Renderer2D
	// @return - Renderer2D* for the instance of an Renderer2D
	static Renderer2D* Get();

	// Initializes SDL, creates the game window, and creates a 2D renderer
	// Enable any OpenGL functionality at the end
	bool Init(int width, int height, bool fullscreen, SDL_bool mouseCaptured, const char* title);

	// De-allocates any resources from SDL
	void Shutdown();

private:
	Renderer2D();
	~Renderer2D();

	// SDL window used for the game
	SDL_Window* mWindow;

	// SDL renderer for 2D graphics
	SDL_Renderer* mRenderer;

	// Title used for the window
	const char* mWindowTitle;

	// Bool for if the renderer is in fullscreen mode
	bool mIsFullScreen;
};
