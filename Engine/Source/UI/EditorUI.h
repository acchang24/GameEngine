#pragma once
#include <SDL2/SDL.h>

class Engine;

class EditorUI
{
public:
	// EditorUI constructor:
	// @param - Engine* for the game engine
	EditorUI(Engine* engine);
	~EditorUI();

	// Initializes the ImGUI library and returns true if intialized
	// @return - bool for if ImGUI initialized properly
	bool Init();

	// Cleans up ImGUI resources
	void Shutdown();

	// Processes any SDL events for the editor's UI
	// @param - const SDL_Event& for the polled event
	void ProcessSDLEvent(const SDL_Event& event);

	void Display();

private:
	// Sets the ImGui style
	void SetImGuiStyle();

	// Pointer to the game engine
	Engine* mEngine;
};
