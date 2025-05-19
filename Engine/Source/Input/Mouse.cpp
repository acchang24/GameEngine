#include "Mouse.h"
#include <iostream>

Mouse::Mouse(double sensitivity) :
	mPosX(0.0),
	mPosY(0.0),
	mSensitivity(sensitivity),
	mCaptured(SDL_TRUE),
	mScrollDir(0),
	mButtonDown(0),
	mButtonUp(0)
{

}

Mouse::Mouse(double sensitivity, SDL_bool capture) :
	mPosX(0.0),
	mPosY(0.0),
	mSensitivity(sensitivity),
	mCaptured(capture),
	mScrollDir(0),
	mButtonDown(0),
	mButtonUp(0)
{
}

Mouse::~Mouse()
{
	std::cout << "Deleted Mouse\n";
}

void Mouse::ToggleMouseCapture()
{
	if (mCaptured == SDL_TRUE)
	{
		mCaptured = SDL_FALSE;
		mPosX = 0;
		mPosY = 0;
	}
	else
	{
		mCaptured = SDL_TRUE;
	}

	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(mCaptured);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);
}

void Mouse::ResetState()
{
	mScrollDir = 0;
	mButtonDown = 0;
	mButtonUp = 0;
}

void Mouse::CalculateMovement()
{
	int x = 0;
	int y = 0;
	SDL_GetRelativeMouseState(&x, &y);
	mPosX = x * mSensitivity;
	mPosY = -y * mSensitivity;
}
