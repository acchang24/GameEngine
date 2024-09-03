#pragma once
#include <vector>
#include <glad/glad.h>
#include <SDL2/SDL.h>

class Shader;
class Texture;
class Entity;
class Camera;
class AssetManager;
class FrameBuffer;
class Skybox;
class Lights;
class ShadowMap;
class JobManager;

class Game
{
public:
	Game();
	~Game();

	// Initializes the game, libraries, and any loads any assets required for the game
	// @return - Returns true if successfully initialized, false if not.
	bool Init();

	// De-allocates any resources and end the game.
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

	void RenderScene();

	void RenderScene(Shader* shader);

	void ProcessMouseInput();

	// Adds an entity to the game's vector of entities
	// @param - Entity* for the new entity
	void AddGameEntity(Entity* e) { mEntities.emplace_back(e); }

private:
	// Array of previous key inputs to see if they are pressed or not
	bool mPrevKeyInputs[256];

	// std::vector of game entities
	std::vector<Entity*> mEntities;

	// SDL window used for the game
	SDL_Window* mWindow;

	// OpenGL context
	SDL_GLContext mContext;

	// Pointer to a (static) asset manager
	AssetManager* mAssetManager;

	// The game's camera
	Camera* mCamera;

	// Frame buffer
	FrameBuffer* mFrameBuffer;

	// Skybox
	Skybox* mSkybox;

	// Use lighting
	Lights* mLights;

	// ShadowMap for shadows
	ShadowMap* mShadowMap;

	// Pointer to a static JobManager
	JobManager* mJobManager;

	double mMousePosX;
	double mMousePosY;

	bool mFirstMouse;

	// Bool to check if the game is running.
	bool mIsRunning;

	bool hdr;

	bool bloom;

	SDL_bool mMouseCaptured;
};
