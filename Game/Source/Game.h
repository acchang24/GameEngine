#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Graphics/Lights.h"
#include "Engine.h"
#include "Util/Console.h"

class AssetManager;
class Entity;
class FrameBuffer;
class FrameBufferMultiSampled;
class SceneManager;
class Shader;
class Skybox;

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
	// @param - AssetManager* for the asset manager
	void LoadShaders(AssetManager* assetManager) const;

	// Loads any assets used by the game
	// @param - AssetManager* for the asset manager
	void LoadAssets(AssetManager* assetManager) const;

	// Loads game models, textures, animations, levels, etc that are specific to this particular game.
	// @param - SceneManager to load entities
	// @param - AssetManager* for the asset manager
	void LoadGameData(SceneManager* sceneManager, AssetManager* assetManager);

	// Runs the main game loop. This will process any user inputs, 
	// updates the game, and renders an output to the screen
	void Run();

	// Processes any keyboard, mouse, or controller inputs.
	// Typically called near the beginning of the render loop.
	// @param - const EngineContext& for the engine context
	void ProcessInput(const EngineContext& engineContext);

	// Updates all the game logic such as player position/movement, enemies, AI, etc.
	// @param - A float representing delta time: the change in time between frames
	// @param - const EngineContext& for the engine context
	void Update(float deltaTime, const EngineContext& engineContext);

	// Sets all the buffers, swap chain, textures, vertex array objects, and renders to screen
	// @param - const EngineContext& for the engine context
	void Render(const EngineContext& engineContext);

	void RenderScene(const EngineContext& engineContext);

	void RenderScene(const EngineContext& engineContext, Shader* shader);

	// Resizes the window, updates viewport, and resizes all frame buffers
	// @param - const SDL_Event& for the resize window event
	// @param - const EngineContext& for the engine context
	void ResizeWindow(const SDL_Event& event, const EngineContext& engineContext);

private:
	// Engine used by game
	Engine mEngine;

	// Console for game info/debugging
	Console mConsole;

	// Game's lighting
	Lights mLights;

	// Skybox
	Skybox* mSkybox;

	// The main multi sampled frame buffer
	FrameBufferMultiSampled* mMainFrameBuffer;
	// Frame buffer for bloom masking
	FrameBuffer* mBloomMaskFrameBuffer;
	// Frame buffer for horizontal blurring
	FrameBuffer* mBloomBlurHorizontalFrameBuffer;
	// Frame buffer for vertical blurring
	FrameBuffer* mBloomBlurVerticalFrameBuffer;
	// Frame buffer for bloom blending
	FrameBuffer* mBloomBlendFrameBuffer;

	std::vector<class Entity*> vampires;

	size_t mShadowIndex;

	// Bool to check if the game is running.
	bool mIsRunning;

	// HDR toggle
	bool hdr;

	// Bloom toggle
	bool bloom;
};
