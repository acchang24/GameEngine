#pragma once
#include <SDL2/SDL.h>

enum class MouseMode
{
	Absolute,	// UI, 2D gameplay, etc
	Relative,	// Used for calculating mouse deltas (FPS camera, free look)
};

class Mouse
{
public:
	Mouse();
	~Mouse();

	// Gets mouse movement by calculating the mouses's x and y axis movement.
	// It will then store the current frame's mouse state
	void CalculateMovement();


	// Mouse Clicks

	// Checks if a mouse button is clicked this frame, but was not clicked the last frame
	// Useful for actions that should only happen once per click, such as shooting a single round per click for a gun, or opening a door, etc.
	// @param - Uint8 for the button to check
	// @return - bool for if the button is clicked this frame, but was not clicked the last frame
	bool IsSingleClick(Uint8 button) const;

	// Checks if a mouse button is held down/clicked
	// @param - Uint8 for the button to check
	// @return - bool for if the button is held down
	bool IsButtonHeld(Uint8 button) const;

	// Checks if a mouse button is released
	// @param - Uint8 for the button to check
	// @return - bool for if the button is released
	bool IsButtonRelease(Uint8 button) const;


	// Toggles between if the mouse is captured or not
	// @param - SDL_Window* for the window if mouse needs to center
	void ToggleMouseCapture(SDL_Window* window);

	// Save the last frame's state and resets the scroll delta
	void ResetState();

	// Centers the mouse in the window
	// @param - SDL_Window* for the window
	void CenterMouse(SDL_Window* window);


	// Gets the mouse's x offset
	// @return - double for x mouse offset
	double GetX() const { return mPosX; }

	// Gets the mouse's y offset
	// @return - double for the y mouse offset
	double GetY() const { return mPosY; }

	// Gets the mouse's sensitivity
	// @return - double for the mouse sensitivity
	double GetSensitivity() const { return mSensitivity; }

	// Gets the mouse's scroll direction
	// @return - Sint32 for the scroll direction
	Sint32 GetScrollDir() const { return mScrollDir; }

	// Gets the mouse's captured status
	// @return - SDL_bool for the mouse capture status
	SDL_bool IsCaptured() const { return mCaptured; }

	// Gets the mouse mode
	// @return - MouseMode for the mode
	MouseMode GetMouseMode() const { return mMode; }


	// Sets the mouse's sensitivity
	// @param - double for the mouse sensitivity
	void SetSensitivity(double sensitivity) { mSensitivity = sensitivity; }

	// Sets the mouse's scroll direction
	// @param - Sint32 for the scroll direction
	void SetScrollDir(Sint32 scrollDir) { mScrollDir = scrollDir; }

	// Sets the mouse's capture status
	// @param - SDL_bool for the status
	void SetCapture(SDL_bool status) { mCaptured = status; }

	// Sets the mouse mode
	// @param - MouseMode for the mode
	void SetMode(MouseMode mode) { mMode = mode; }

private:
	// x-axis movement
	double mPosX;

	// y-axis movement
	double mPosY;

	// Mouse sensitivity
	double mSensitivity;

	// Current frame's mouse state
	Uint32 mCurrentState;

	// Previous frame's mouse state
	Uint32 mPreviousState;

	// Mouse scroll delta representing the direction
	Sint32 mScrollDir;

	// Bool for if the mouse is captured by the window
	SDL_bool mCaptured;

	// Mouse mode (relative or absolute)
	MouseMode mMode;
};
