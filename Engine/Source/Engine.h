#pragma once
#include <SDL2/SDL.h>
#include "Audio/AudioSystem.h"
#include "Graphics/Renderer.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Physics/Physics.h"
#include "UI/EditorUI.h"
#include "Util/Logger.h"

class AssetManager;
class Console;
class JobManager;

// Engine class is the central system for game framework. 
// Manages core subsystems like rendering, audio, input, asset loading,
// multithreading, physics, logging, and profiling. Provides initialization
// and shutdown of all game engine systems.
class Engine
{
public:
	// Engine constructor: creates subsystem objects and starts logger
	// @param - RendererMode for 2D or 3D game
	// @param - double for initial mouse sensitivity
	Engine(RendererMode renderMode, double mouseSensitivity);
	~Engine();

	// Initializes the game engine subsystems. Returns true after successful initialization
	// @param - int for initial window width
	// @param - int for initial window height
	// @param - int for initial number of sub samples used for anti aliasing (1, 2, 4, 8)
	// @param - int for initial v sync (0 = immediate, 1 = sync, -1 = adaptive
	// @param - bool for if the window is fullscreen
	// @param - SDL_bool for if the mouse is captured by the screen
	// @param - const char* for the game name
	// @return - bool for if the engine was initialized properly
	bool Init(int windowWidth, int windowHeight, int subSamples, int v_sync, bool fullscreen, SDL_bool mouseCaptured, const char* gameName);

	// De-allocates any resources
	void Shutdown();

	// Gets the engine's renderer
	// @return - Renderer* for the renderer
	Renderer* GetRenderer() { return &mRenderer; }

	// Gets the keyboard
	// @return - Keyboard* for the keyboard
	Keyboard* GetKeyboard() { return &mKeyboard; }

	// Gets the logger
	// @return - Logger* for the logger
	Logger* GetLogger() { return &mLogger; }

	// Gets the mouse
	// @return - Mouse* for the mouse
	Mouse* GetMouse() { return &mMouse; }

	// Gets the physics system
	// @return - Physics* for the physics system
	Physics* GetPhysics() { return &mPhysics; }

	// Gets the EditorUI system
	// @return - EditorUI* for the editor
	EditorUI* GetEditorUI() { return &mEditorUI; }

	// Gets the audio system
	// @return - Audio* for the audio
	AudioSystem* GetAudio() { return &mAudio; }

	Console* GetConsole() { return mConsole; }

private:
	// Renderer for 2D/3D graphics output
	Renderer mRenderer;

	// Keyboard
	Keyboard mKeyboard;

	// Logger to log messages
	Logger mLogger;

	// Mouse
	Mouse mMouse;

	// Physics system
	Physics mPhysics;

	// Pointer to a static JobManager for multi threading
	JobManager* mJobManager;

	// Pointer to a static AssetManager to load/cache assets on demand
	AssetManager* mAssetManager;

	// Editor UI system
	EditorUI mEditorUI;

	// Audio system
	AudioSystem mAudio;

	Console* mConsole;
};
