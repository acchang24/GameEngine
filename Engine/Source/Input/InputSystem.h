#pragma once
#include <SDL2/SDL.h>

enum class MouseMode
{
	Absolute,	// UI, 2D gameplay, etc
	Relative,	// Used for calculating mouse deltas (FPS camera, free look)
};

class InputSystem
{
public:
	InputSystem(SDL_Window* window, double mouseSensitivity, SDL_bool capture);

	~InputSystem();

	// Saves the keystate from the previous frame, resets the mouse state, and gets the current keystate and mouse movement for the current frame
	// This should be called at the beginning of the game loop's Game::ProcessInput() function
	void StartFrame();

	// Clears temporary, non-persistent states right before the frame changes. 
	// This shuld be called at the very end of the game loop's Game::ProcessInput() function
	void EndFrame();


	void HandleEvent(const SDL_Event& event);



	// KEYBOARD API
	// Checks if a key is pressed
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is pressed
	bool IsKeyPressed(SDL_Scancode code) const;

	// Checks if a key is released
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is released
	bool IsKeyReleased(SDL_Scancode code) const;

	// Checks if a key is pressed this frame, but was not pressed the last frame
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is pressed this frame, but was not pressed the last frame
	bool IsKeyLeadingEdge(SDL_Scancode code) const;

	// Resets the keyboard state
	void ResetKeyboard() const;


	// MOUSE API
	// Checks if a mouse button is clicked this frame, but was not clicked the last frame
	// @param - Uint8 for the button to check
	// @return - bool for if the button is clicked this frame, but was not clicked the last frame
	bool IsMouseSingleClick(Uint8 button) const;

	// Checks if a mouse button is held down/clicked
	// @param - Uint8 for the button to check
	// @return - bool for if the button is held down
	bool IsMouseHeld(Uint8 button) const;

	// Checks if a mouse button is released
	// @param - Uint8 for the button to check
	// @return - bool for if the button is released
	bool IsMouseRelease(Uint8 button) const;


	// Toggles the mouse capture mode
	void ToggleMouseCapture();

	// Centers the mouse in the window
	void CenterMouse();

private:
	// Current frame's keyboard state
	Uint8 mCurrentKeyState[SDL_NUM_SCANCODES];

	// Previous frame's keyboard state
	Uint8 mPrevKeyState[SDL_NUM_SCANCODES];

	// Pointer to the game window
	SDL_Window* mWindow;


	// Current frame's mouse state
	Uint32 mMouseState;

	// Previous frame's mouse state
	Uint32 mPrevMouseState;

	// Mouse scroll direction
	Sint32 mScrollDir;

	// Mouse x movement
	double mMouseX;

	// Mouse y movement
	double mMouseY;

	// Mouse sensitivity
	double mMouseSensitivity;

	// Mouse mode (relative or absolute)
	MouseMode mMouseMode;

	// Bool for if the mouse is captured by the window
	SDL_bool mMouseCaptured;
};
