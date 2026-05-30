#pragma once
#include <SDL2/SDL.h>
#include "Audio/AudioSystem.h"
#include "EngineUI/EngineUI.h"
#include "Graphics/Renderer.h"
#include "Input/InputSystem.h"
#include "Physics/Physics.h"
#include "Util/Logger.h"

class AssetManager;
class Console;
class Editor;
class JobManager;

struct EngineContext
{
	Renderer* renderer = nullptr;
	InputSystem* input = nullptr;
	Physics* physics = nullptr;
	JobManager* jobManager = nullptr;
	AssetManager* assetManager = nullptr;
	EngineUI* engineUI = nullptr;
	AudioSystem* audio = nullptr;
};

// Engine class is the central system for game framework. 
// Manages core subsystems like rendering, audio, input, asset loading,
// multithreading, physics, logging, and profiling. Provides initialization
// and shutdown of all game engine systems.
class Engine
{
public:
	// Engine constructor: creates subsystem objects and starts logger
	// @param - RendererMode for 2D or 3D game
	Engine(RendererMode renderMode);
	~Engine();

	// Initializes the game engine subsystems. Returns true after successful initialization
	// @param - int for initial window width
	// @param - int for initial window height
	// @param - int for initial number of sub samples used for anti aliasing (1, 2, 4, 8)
	// @param - int for initial v sync (0 = immediate, 1 = sync, -1 = adaptive
	// @param - bool for if the window is fullscreen
	// @param - double for the mouse sensitivity
	// @param - SDL_bool for if the mouse is captured by the screen
	// @param - const char* for the game name
	// @return - bool for if the engine was initialized properly
	bool Init(int windowWidth, int windowHeight, int subSamples, int v_sync, bool fullscreen, double mouseSensitivity, SDL_bool mouseCaptured, const char* gameName);

	// De-allocates any resources
	void Shutdown();

	// Gets the engine's renderer
	// @return - Renderer* for the renderer
	Renderer* GetRenderer() { return &mRenderer; }

	// Gets the engine's input system
	// @return - InputSystem* for the input system
	InputSystem* GetInputSystem() { return &mInputSystem; }

	// Gets the physics system
	// @return - Physics* for the physics system
	Physics* GetPhysics() { return &mPhysics; }

	// Gets the EngineUI system
	// @return - EngineUI* for the engine ui
	EngineUI* GetEngineUI() { return &mEngineUI; }

	// Gets the audio system
	// @return - Audio* for the audio
	AudioSystem* GetAudio() { return &mAudio; }

	Console* GetConsole() { return mConsole; }

	//Editor* GetEditor() { return mEditor; }

private:
	// Renderer for 2D/3D graphics output
	Renderer mRenderer;

	// Input system
	InputSystem mInputSystem;

	// Physics system
	Physics mPhysics;

	// Pointer to a static JobManager for multi threading
	JobManager* mJobManager;

	// Pointer to a static AssetManager to load/cache assets on demand
	AssetManager* mAssetManager;

	// Logger to log messages
	Logger* mLogger;

	// Engine UI system
	EngineUI mEngineUI;

	// Audio system
	AudioSystem mAudio;

	Console* mConsole;

	//Editor* mEditor;
};
