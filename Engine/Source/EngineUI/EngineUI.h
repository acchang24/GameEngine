#pragma once
#include <SDL2/SDL.h>

class Engine;

class EngineUI
{
public:
	// EditorUI constructor:
	// @param - Engine* for the game engine
	EngineUI(Engine* engine);
	~EngineUI();

	// Initializes the ImGUI library and returns true if intialized
	// @return - bool for if ImGUI initialized properly
	bool Init();

	// Cleans up ImGUI resources
	void Shutdown();

	// Processes any SDL events for the engine's UI
	// @param - const SDL_Event& for the polled event
	void ProcessSDLEvent(const SDL_Event& event);

	void SetUI();

	void Render();

private:
	// Sets the ImGui style
	void SetImGuiStyle();

	// Pointer to the game engine
	Engine* mEngine;

	// Pointer to the window
	SDL_Window* mWindow;
};
