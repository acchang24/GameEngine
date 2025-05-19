#pragma once
#include <SDL2/SDL.h>

class Mouse
{
public:
	// Mouse constructor:
	// @param - double for the mouses's sensitivity
	Mouse(double sensitivity);
	// Mouse constructor:
	// @param - double for the mouses's sensitivity
	// @param - SDL_bool for if the mouse is captured or not
	Mouse(double sensitivity, SDL_bool capture);
	~Mouse();

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
	SDL_bool MouseCaptured() const { return mCaptured; }

	// Gets the mouse's scroll direction
	// @return - Sint32 for the scroll direction
	Sint32 GetScrollDir() const { return mScrollDir; }

	// Get the mouse's button down state
	// @return - Uint8 for the button state
	Uint8 GetButtonDown() const { return mButtonDown; }

	// Get the mouse's button up state
	// @return - Uint8 for the button state
	Uint8 GetButtonUp() const { return mButtonUp; }

	// Sets the mouse's x offset
	// @param - double for the x mouse offset
	void SetX(double x) { mPosX = x; }

	// Sets the mouse's y offset
	// @param - double for the y mouse offset
	void SetY(double y) { mPosY = y; }

	// Sets the mouse's sensitivity
	// @param - double for the mouse sensitivity
	void SetSensitivity(double sensitivity) { mSensitivity = sensitivity; }

	// Sets the mouse's capture status
	// @param - SDL_bool for the status
	void SetCapture(SDL_bool status) { mCaptured = status; }

	// Sets the mouse's scroll dir
	// @param - Sint32 for the scroll dir
	void SetScrollDir(Sint32 dir) { mScrollDir = dir; }

	// Sets the mouse's button down state
	// @param - Uint8 for the button state
	void SetButtonDown(Uint8 down) { mButtonDown = down; }

	// Sets the mouse's button up state
	// @param - Uint8 for the button state
	void SetButtonUp(Uint8 up) { mButtonUp = up; }

	// Toggles between if the mouse is captured or not
	void ToggleMouseCapture();

	// Sets all button states back to 0
	void ResetState();

	// Calculates the mouses's x and y axis movement
	void CalculateMovement();

private:
	// x-axis movement
	double mPosX;

	// y-axis movement
	double mPosY;

	// Mouse sensitivity
	double mSensitivity;

	// Bool for if the mouse is captured by the window
	SDL_bool mCaptured;

	// Scroll direction
	Sint32 mScrollDir;

	// Mouse button down
	Uint8 mButtonDown;

	// Mouse button up
	Uint8 mButtonUp;
};
