#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Audio/AudioSystem.h"
#include "Graphics/Camera.h"
#include "Graphics/Lights.h"
#include "Graphics/Renderer.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Physics/Physics.h"

class AssetManager;
class Entity;
class JobManager;
class AABBComponent2D;
class Asteroid;

// Game class handles all of the game logic. Game specific code should be added to this class
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

	// Loads any shaders used for the game
	void LoadShaders() const;

	// Loads game models, textures, animations, levels, etc that are specific to this particular game.
	void LoadGameData();

	// Deletes all entities, sounds, levels, etc
	void UnloadGameData();

	// Runs the main game loop. This will process any user inputs, 
	// updates the game, and renders an output to the screen
	void Run();

	// Processes any keyboard, mouse, or controller inputs.
	// Typically called near the beginning of the render loop.
	void ProcessInput();

	// Processes and handles any mouse movement, clicks, and scrolls
	// @param - Mouse* for the mouse
	void ProcessMouseInput(Mouse* mouse);

	// Updates all the game logic such as player position/movement, enemies, AI, etc.
	// @param - A float representing delta time: the change in time between frames
	void Update(float deltaTime);

	// Sets all the buffers, swap chain, textures, vertex array objects, and renders to screen
	void Render();

	// Resizes the window, updates viewport, and resizes all frame buffers
	// @param - const SDL_Event& for the resize window event
	void ResizeWindow(const SDL_Event& event);

	// Adds an entity to the game's vector of entities
	// @param - Entity* for the new entity
	void AddGameEntity(Entity* e) { mEntities.emplace_back(e); }

	// Removes an entity from the game's vector of entities, and deletes that entity from memory
	// @param - Entity* for the entity to remove
	void RemoveGameEntity(Entity* e);

	// Gets the game's keyboard
	// @return - Keyboard* for the game's keyboard
	Keyboard* GetKeyboard() { return &mKeyboard; }

	// Gets the game's physics system
	// @return - Physics* for the game physics
	Physics* GetPhysics() { return &mPhysics; }

	// Gets the game's audio system
	// @return - AudioSystem* for the audio system
	AudioSystem* GetAudio() { return &mAudio; }

private:
	// std::vector of game entities
	std::vector<Entity*> mEntities;

	// Renderer for graphics output
	Renderer mRenderer;

	// Pointer to a static JobManager
	JobManager* mJobManager;

	// Pointer to a static asset manager
	AssetManager* mAssetManager;

	// Game's keyboard input
	Keyboard mKeyboard;

	// Game's mouse input
	Mouse mMouse;

	// Game's physics
	Physics mPhysics;

	// Audio system
	AudioSystem mAudio;

	// Bool to check if the game is running.
	bool mIsRunning;
};
