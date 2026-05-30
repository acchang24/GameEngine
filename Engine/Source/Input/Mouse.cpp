#include "Mouse.h"
#include <algorithm>
#include <iostream>

Mouse::Mouse() :
	mPosX(0.0),
	mPosY(0.0),
	mSensitivity(0.0),
	mCurrentState(0),
	mPreviousState(0),
	mScrollDir(0),
	mCaptured(SDL_FALSE),
	mMode(MouseMode::Absolute)
{
	
}

Mouse::~Mouse()
{
	std::cout << "Deleted Mouse\n";
}

void Mouse::CalculateMovement()
{
	int x = 0;
	int y = 0;
	if (mMode == MouseMode::Relative)
	{
		mCurrentState = SDL_GetRelativeMouseState(&x, &y);

		// Apply sensitivity and flip y-axis for typical screen coordinates
		mPosX = x * mSensitivity;
		mPosY = -y * mSensitivity;
	}
	else
	{
		mCurrentState = SDL_GetMouseState(&x, &y);
		mPosX = x;
		mPosY = y;
	}
}

bool Mouse::IsSingleClick(Uint8 button) const
{
	// Current frame click state for the button
	bool isClicked = (mCurrentState & SDL_BUTTON(button)) != 0;

	// Previous frame click state for the button
	bool prevClick = (mPreviousState & SDL_BUTTON(button)) != 0;

	return isClicked && !prevClick;
}

bool Mouse::IsButtonHeld(Uint8 button) const
{
	return ((mCurrentState & SDL_BUTTON(button)) != 0);
}

bool Mouse::IsButtonRelease(Uint8 button) const
{
	// Current frame click state for the button
	bool isClicked = (mCurrentState & SDL_BUTTON(button)) != 0;

	// Previous frame click state for the button
	bool prevClick = (mPreviousState & SDL_BUTTON(button)) != 0;

	return !isClicked && prevClick;
}

void Mouse::ToggleMouseCapture(SDL_Window* window)
{
	if (mCaptured == SDL_TRUE)
	{
		mMode = MouseMode::Absolute;
		mCaptured = SDL_FALSE;
	}
	else
	{
		mMode = MouseMode::Relative;
		mCaptured = SDL_TRUE;
	}

	mPosX = 0;
	mPosY = 0;

	CenterMouse(window);

	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(mCaptured);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);
}

void Mouse::ResetState()
{
	// Save the last frame's mouse state
	mPreviousState = mCurrentState;

	// Reset the scroll delta for new frame
	mScrollDir = 0;
}

void Mouse::CenterMouse(SDL_Window* window)
{
	if (window)
	{
		int windowW = 0;
		int windowH = 0;
		SDL_GetWindowSize(window, &windowW, &windowH);
		SDL_WarpMouseInWindow(window, windowW / 2, windowH / 2);
	}
}
