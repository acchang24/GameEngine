#pragma once
#include <SDL2/SDL.h>

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	// Gets the keyboard's state
	const Uint8* GetState();

	// Detects if a key was pressed in the last frame
	bool PressedLastFrame(SDL_Scancode code) const;

	// Detects if a key is currently pressed, while being unpressed in the last frame
	bool HasLeadingEdge(const Uint8* state, SDL_Scancode code) const;

	// Saves a key's state for reference in the next frame (use at end of ProcessInput)
	void SavePrevKeyState(const Uint8* state, SDL_Scancode code);

	void ResetKeyboard();

private:
	// Array of previous key inputs to see if they are pressed or not
	bool mPrevKeyInputs[SDL_NUM_SCANCODES / 2];
};
