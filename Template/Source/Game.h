#pragma once
#include <vector>
#include <SDL2/SDL.h>

class Entity;

// Game class handles all of the game logic. Game specific code should be added to the class
class Game
{
public:
	Game();
	~Game();

	// Initializes the game, libraries, and any loads any assets required for the game
	// @return - Returns true if successfully initialized, false if not.
	bool Init();

	// Initializes SDL, sets OpenGL attributes, creates the game window, creates the OpenGL context, and sets up GLAD.
	// Enable any OpenGL functionality at the end
	bool SetupOpenGL();

	// Loads game models, textures, animations, levels, etc that are specific to this particular game.
	bool LoadGameData();

	// De-allocates any resources and ends the game.
	void Shutdown();

	// Runs the main game loop.
	void Run();

	// Processes any keyboard, mouse, or controller inputs.
	// Typically called near the beginning of the render loop.
	void ProcessInput();
	
	// Updates all the game logic such as player position/movement, enemies, AI, etc.
	// @param - A float representing delta time: the change in time between frames
	void Update(float deltaTime);

	// Sets all the buffers, swap chain, textures, vertex array objects, and renders to screen
	void Render();

private:
	// std::vector of game entities
	std::vector<Entity*> mEntities;

	// Array of previous key inputs to see if they are pressed or not
	bool mPrevKeyInputs[256];

	// SDL window used for the game
	SDL_Window* mWindow;

	// OpenGL context
	SDL_GLContext mContext;

	// Game's mouse position along X-axis
	double mMousePosX;
	// Game's mouse position along Y-axis
	double mMousePosY;

	// Bool for the games's running state
	bool mIsRunning;
};
