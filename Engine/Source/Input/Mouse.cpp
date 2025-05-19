#include "Mouse.h"
#include <iostream>

Mouse::Mouse() :
	mMousePosX(0.0),
	mMousePosY(0.0),
	mMouseCaptured(SDL_TRUE),
	mScrollDir(0),
	mMouseButtonDown(0),
	mMouseButtonUp(0)
{

}

Mouse::~Mouse()
{
	std::cout << "Deleted Mouse\n";
}

void Mouse::ToggleMouseCapture()
{
	if (mMouseCaptured == SDL_TRUE)
	{
		mMouseCaptured = SDL_FALSE;
		mMousePosX = 0;
		mMousePosY = 0;
	}
	else
	{
		mMouseCaptured = SDL_TRUE;
	}

	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(mMouseCaptured);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);
}

void Mouse::ResetState()
{
	mScrollDir = 0;
	mMouseButtonDown = 0;
	mMouseButtonUp = 0;
}
