#pragma once
#include <SDL2/SDL.h>
#include <cstring>

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	// Checks if a key is pressed
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is pressed
	bool IsKeyPressed(SDL_Scancode code) const { return mCurrentKeyState[code] != 0; }

	// Checks if a key is released
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is released
	bool IsKeyReleased(SDL_Scancode code) const { return mCurrentKeyState[code] == 0; }

	// Checks if a key is pressed this frame, but was not pressed the last frame
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is pressed this frame, but was not pressed the last frame
	bool IsKeyLeadingEdge(SDL_Scancode code) const { return ((mCurrentKeyState[code] != 0) && (mPrevKeyState[code] == 0)); }

	// Saves the current frame's key state
	void SaveCurrentKeyState(const Uint8* state) { std::memcpy(mCurrentKeyState, state, sizeof(Uint8) * SDL_NUM_SCANCODES); }

	// Saves the previous frame's key state
	void SavePrevKeyState() { std::memcpy(mPrevKeyState, mCurrentKeyState, sizeof(Uint8) * SDL_NUM_SCANCODES); }
	
	// Resets the keyboard state
	void ResetKeyboard() const { SDL_ResetKeyboard(); }

private:
	// Current frame's keyboard state
	Uint8 mCurrentKeyState[SDL_NUM_SCANCODES];

	// Previous frame's keyboard state
	Uint8 mPrevKeyState[SDL_NUM_SCANCODES];
};
