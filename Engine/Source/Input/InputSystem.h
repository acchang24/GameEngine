#pragma once
#include <SDL2/SDL.h>
#include "Keyboard.h"
#include "Mouse.h"

class InputSystem
{
public:
	InputSystem();

	~InputSystem();

	// Initializes the input system, setting the window
	// @param - SDL_Window* for the window
	// @param - double for the mouse sensitivity
	// @param - SDL_bool for if the mouse is captured by the window
	// @return - bool for if the input system was initialized properly
	bool Init(SDL_Window* window, double mouseSensitivity, SDL_bool capture);

	// Saves the keystate from the previous frame, resets the mouse state
	// This should be called at the beginning of the game loop's Game::ProcessInput() function
	void StartFrame();

	// Gets the current snapshot of keyboard and mouse movement
	void GetCurrentState();

	// Clears temporary, non-persistent states right before the frame changes. 
	// This shuld be called at the very end of the game loop's Game::ProcessInput() function
	void EndFrame();

	// Handles any events that are polled, such as mouse scroll events
	// @param - const SDL_Event& for the event to handle
	void HandleEvent(const SDL_Event& event);


	// KEYBOARD API
	
	// Checks if a key is pressed
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is pressed
	bool IsKeyPressed(SDL_Scancode code) const { return mKeyboard.IsKeyPressed(code); }

	// Checks if a key is released
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is released
	bool IsKeyReleased(SDL_Scancode code) const { return mKeyboard.IsKeyReleased(code); }

	// Checks if a key is pressed this frame, but was not pressed the last frame
	// @param - SDL_Scancode for the key to check
	// @return - bool for if the key is pressed this frame, but was not pressed the last frame
	bool IsKeyLeadingEdge(SDL_Scancode code) const { return mKeyboard.IsKeyLeadingEdge(code); }

	// Resets the keyboard state
	void ResetKeyboard() const { mKeyboard.ResetKeyboard(); }



	// MOUSE API
	
	// Checks if a mouse button is clicked this frame, but was not clicked the last frame
	// @param - Uint8 for the button to check
	// @return - bool for if the button is clicked this frame, but was not clicked the last frame
	bool IsMouseSingleClick(Uint8 button) const { return mMouse.IsSingleClick(button); }

	// Checks if a mouse button is held down/clicked
	// @param - Uint8 for the button to check
	// @return - bool for if the button is held down
	bool IsMouseHeld(Uint8 button) const { return mMouse.IsButtonHeld(button); }

	// Checks if a mouse button is released
	// @param - Uint8 for the button to check
	// @return - bool for if the button is released
	bool IsMouseRelease(Uint8 button) const { return mMouse.IsButtonRelease(button); }

	// Gets the mouse x movement for the current frame
	// @return - double for the mouse x movement
	double GetMouseX() const { return mMouse.GetX(); }

	// Gets the mouse y movement for the current frame
	// @return - double for the mouse y movement
	double GetMouseY() const { return mMouse.GetY();; }

	// Gets the mouse scroll direction
	// @return - Sint32 for the scroll direction
	Sint32 GetMouseScrollDir() const { return mMouse.GetScrollDir(); }

	// Toggles the mouse capture mode
	void ToggleMouseCapture() { mMouse.ToggleMouseCapture(mWindow); }

	// Centers the mouse in the window
	void CenterMouse() { mMouse.CenterMouse(mWindow); }

	// Returns true if mouse is captured by the window, false if not
	// @return - bool for if the mouse is captured by the window
	bool MouseIsCaptured() const { return mMouse.IsCaptured(); }

private:
	// Keyboard device
	Keyboard mKeyboard;

	// Mouse device
	Mouse mMouse;

	// Pointer to the game window
	SDL_Window* mWindow;
};
