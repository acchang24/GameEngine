#include "Mouse.h"
#include <algorithm>
#include <iostream>

Mouse::Mouse(double sensitivity, SDL_bool capture) :
	mPosX(0.0),
	mPosY(0.0),
	mSensitivity(sensitivity),
	mState(0),
	mButtonsDown(),
	mButtonsUp(),
	mCaptured(capture),
	mMouseMode(GetModeFromCapture(capture)),
	mScrollDir(0)
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
	if (mMouseMode == MouseMode::Relative)
	{
		mState = SDL_GetRelativeMouseState(&x, &y);
		mPosX = x * mSensitivity;
		mPosY = -y * mSensitivity;
	}
	else
	{
		mState = SDL_GetMouseState(&x, &y);
		mPosX = x;
		mPosY = y;
	}
}

void Mouse::ToggleMouseCapture(SDL_Window* window)
{
	if (mCaptured == SDL_TRUE)
	{
		mMouseMode = MouseMode::Absolute;
		mCaptured = SDL_FALSE;
	}
	else
	{
		mMouseMode = MouseMode::Relative;
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
	mScrollDir = 0;
	std::fill(std::begin(mButtonsDown), std::end(mButtonsDown), false);
	std::fill(std::begin(mButtonsUp), std::end(mButtonsUp), false);
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

MouseMode Mouse::GetModeFromCapture(SDL_bool mouseCaptured)
{
	if (mouseCaptured == SDL_TRUE)
	{
		return MouseMode::Relative;
	}
	else
	{
		return MouseMode::Absolute;
	}
}
