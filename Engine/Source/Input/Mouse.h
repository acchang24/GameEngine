#pragma once
#include <SDL2/SDL.h>

const size_t MAX_MOUSE_BUTTONS = 6;

enum class MouseMode
{
	Absolute,	// UI, 2D gameplay, etc
	Relative,	// Used for calculating mouse deltas (FPS camera, free look)
};

class Mouse
{
public:
	// Mouse constructor:
	// @param - double for the mouses's sensitivity
	// @param - SDL_bool for if the mouse is captured or not
	Mouse(double sensitivity, SDL_bool capture);
	~Mouse();

	// Gets mouse state and calculates the mouses's x and y axis movement
	void CalculateMovement();

	// Toggles between if the mouse is captured or not
	// @param - SDL_Window* for the window if mouse needs to center
	void ToggleMouseCapture(SDL_Window* window);

	// Sets all button states back to 0
	void ResetState();

	// Centers the mouse in the window
	// @param - SDL_Window* for the window
	void CenterMouse(SDL_Window* window);

	// Gets the mouse state
	// @return - Uint8 for the mouse state
	Uint8 GetState() const { return mState; }

	// Gets the mouse's x offset
	// @return - double for x mouse offset
	double GetX() const { return mPosX; }

	// Gets the mouse's y offset
	// @return - double for the y mouse offset
	double GetY() const { return mPosY; }

	// Gets the mouse's sensitivity
	// @return - double for the mouse sensitivity
	double GetSensitivity() const { return mSensitivity; }

	// Gets the mouse's captured status
	// @return - SDL_bool for the mouse capture status
	SDL_bool MouseIsCaptured() const { return mCaptured; }

	// Gets the mouse mode
	// @return - MouseMode for the mode
	MouseMode GetMouseMode() const { return mMouseMode; }

	// Gets the mouse's scroll direction
	// @return - Sint32 for the scroll direction
	Sint32 GetScrollDir() const { return mScrollDir; }

	// Get the mouse's button down state
	// @return - Uint8 for the button state
	bool IsButtonDown(Uint8 button) const { return mButtonsDown[button]; }

	// Get the mouse's button up state
	// @return - Uint8 for the button state
	bool IsButtonUp(Uint8 button) const { return mButtonsUp[button]; }

	// Sets the mouse's sensitivity
	// @param - double for the mouse sensitivity
	void SetSensitivity(double sensitivity) { mSensitivity = sensitivity; }

	// Sets the mouse's capture status
	// @param - SDL_bool for the status
	void SetCapture(SDL_bool status) { mCaptured = status; }

	// Sets the mouse's scroll dir
	// @param - Sint32 for the scroll dir
	void SetScrollDir(Sint32 dir) { mScrollDir = dir; }

	// Sets the mouse's button down state for the specified button
	// @param - Uint8 for the specified button
	void SetButtonDown(Uint8 down) { mButtonsDown[down] = true; }

	// Sets the mouse's button up state for the specified button
	// @param - Uint8 for the specified button
	void SetButtonUp(Uint8 up) { mButtonsUp[up] = true; }

private:
	// Gets the mouse mode from the mouse captured status
	// @param - SDL_bool for mouse captured
	MouseMode GetModeFromCapture(SDL_bool mouseCaptured);
	
	// x-axis movement
	double mPosX;

	// y-axis movement
	double mPosY;

	// Mouse sensitivity
	double mSensitivity;

	// Mouse state (for button holds)
	Uint8 mState;

	// Mouse button down
	bool mButtonsDown[MAX_MOUSE_BUTTONS];

	// Mouse button up
	bool mButtonsUp[MAX_MOUSE_BUTTONS];

	// Bool for if the mouse is captured by the window
	SDL_bool mCaptured;

	// Mouse movement mode
	MouseMode mMouseMode;

	// Scroll direction
	Sint32 mScrollDir;
};
