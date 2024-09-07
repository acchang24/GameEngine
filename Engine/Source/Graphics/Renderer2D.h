#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>

class SpriteComponent;

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

	// Checks for window resize, clears the screen, and clears the color and depth buffer bits
	void ClearBuffers();

	// Draws the sprites to the screen
	void Draw();

	// Swap the buffers and present to the screen
	void EndFrame();

	// Gets the SDL renderer
	// @return - SDL_Renderer* for the 2D sdl renderer
	SDL_Renderer* GetSdlRenderer() { return m2dRenderer; }

	// Loads a sprite texture given a file name. First checks to see if it
	// is already loaded in the mSpriteTextureMap. If it is there, simply load
	// that sprite texture. If not, it will load the image using SDL and adds to sprite map
	// @param - const std::string& for the file name
	SDL_Texture* LoadSpriteTexture(const std::string& fileName);

	// Adds a sprite texture and to the sorted vector of sprites
	// @param - SpriteComponent* for the new sprite to add
	void AddSprite(SpriteComponent* sprite);

	// Removes a sprite texture from the sorted vector of sprites
	// @param - SpriteComponent* for the sprite to remove
	void RemoveSprite(SpriteComponent* sprite);

	static int GetWidth();
	static int GetHeight();

private:
	Renderer2D();
	~Renderer2D();

	// Map of all sprite textures
	std::unordered_map<std::string, SDL_Texture*> mSpriteTextureMap;

	// Vector of all sprites (components) sorted by draw order
	std::vector<SpriteComponent*> mSprites;

	// SDL window used for the game
	SDL_Window* mWindow;

	// SDL renderer for 2D graphics
	SDL_Renderer* m2dRenderer;

	// Title used for the window
	const char* mWindowTitle;

	// Bool for if the renderer is in fullscreen mode
	bool mIsFullScreen;
};
