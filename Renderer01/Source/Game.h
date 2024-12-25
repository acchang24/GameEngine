#pragma once
#include <vector>
#include <SDL2/SDL.h>

class AssetManager;
class Entity;
class Renderer3D;
class Shader;
class VertexBuffer;

// Game class handles all of the game logic. Game specific code should be added to the class
class Game
{
public:
	Game();
	~Game();

	// Initializes the game, libraries, and any loads any assets required for the game
	// @return - Returns true if successfully initialized, false if not.
	bool Init();

	// De-allocates any resources and ends the game.
	void Shutdown();

	// Loads game models, textures, animations, levels, etc that are specific to this particular game.
	bool LoadGameData();

	// Deletes all entities, sounds, levels, etc
	void UnloadGameData();

	// Runs the main game loop. This will process any user inputs, 
	// updates the game, and renders an output to the screen
	void Run();

	// Processes any keyboard, mouse, or controller inputs.
	// Typically called near the beginning of the render loop.
	void ProcessInput();

	// Processes and handles any mouse movement, clicks, and scrolls
	// @param - Uint8 for click down button
	// @param - Uint8 for click release button
	// @param - Sint32 for scroll direction
	void ProcessMouseInput(Uint8 buttonDown, Uint8 buttonUp, Sint32 scroll);
	
	// Updates all the game logic such as player position/movement, enemies, AI, etc.
	// @param - A float representing delta time: the change in time between frames
	void Update(float deltaTime);

	// Sets all the buffers, swap chain, textures, vertex array objects, and renders to screen
	void Render();

	// Adds an entity to the game's vector of entities
	// @param - Entity* for the new entity
	void AddGameEntity(Entity* e) { mEntities.emplace_back(e); }

	// Removes an entity from the game's vector of entities, and deletes that entity from memory
	// @param - Entity* for the entity to remove
	void RemoveGameEntity(Entity* e);

private:
	// Array of previous key inputs to see if they are pressed or not
	bool mPrevKeyInputs[256];

	// std::vector of game entities
	std::vector<Entity*> mEntities;

	// Vertex Buffer
	VertexBuffer* mVertexBuffer;

	// Shader for triangle
	Shader* mShader;

	// Renderer for graphics output
	Renderer3D* mRenderer;

	// Pointer to a static asset manager
	AssetManager* mAssetManager;

	// Mouse x-axis movement
	double mMousePosX;
	// Mouse y-axis movement
	double mMousePosY;

	// Bool to check if the game is running.
	bool mIsRunning;

	// SDL_bool for if the mouse is captured by the screen
	SDL_bool mMouseCaptured;
};
