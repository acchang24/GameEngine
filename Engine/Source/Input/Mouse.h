#pragma once
#include <SDL2/SDL.h>

class Mouse
{
public:
	Mouse();
	~Mouse();

	// Gets the mouse's x offset
	// @return - double for x mouse offset
	double GetMouseX() const { return mMousePosX; }

	// Gets the mouse's y offset
	// @return - double for the y mouse offset
	double GetMouseY() const { return mMousePosY; }

	// Gets the mouse's captured status
	// @return - SDL_bool for the mouse capture status
	SDL_bool MouseCaptured() const { return mMouseCaptured; }

	// Get the mouse's button down state
	// @return - Uint8 for the button state
	Uint8 GetMouseButtonDown() const { return mMouseButtonDown; }

	// Get the mouse's button up state
	// @return - Uint8 for the button state
	Uint8 GetMouseButtonUp() const { return mMouseButtonUp; }

	// Sets the mouse's x offset
	// @param - double for the x mouse offset
	void SetMouseX(double x) { mMousePosX = x; }

	// Sets the mouse's y offset
	// @param - double for the y mouse offset
	void SetMouseY(double y) { mMousePosY = y; }

	// Sets the mouse's capture status
	// @param - SDL_bool for the status
	void SetMouseCapture(SDL_bool status) { mMouseCaptured = status; }

	// Sets the mouse's button down state
	// @param - Uint8 for the button state
	void SetMouseButtonDown(Uint8 down) { mMouseButtonDown = down; }

	// Sets the mouse's button up state
	// @param - Uint8 for the button state
	void SetMouseButtonUp(Uint8 up) { mMouseButtonUp = up; }

	// Toggles between if the mouse is captured or not
	void ToggleMouseCapture();

	// Sets all button states back to 0
	void ResetState();

private:
	// Mouse x-axis movement
	double mMousePosX;

	// Mouse y-axis movement
	double mMousePosY;

	// Bool for if the mouse is captured by the window
	SDL_bool mMouseCaptured;

	// Scroll direction
	Sint32 mScrollDir;

	// Mouse button down
	Uint8 mMouseButtonDown;

	// Mouse button up
	Uint8 mMouseButtonUp;
};
