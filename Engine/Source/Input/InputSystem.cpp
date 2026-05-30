#include "InputSystem.h"
#include <cstring>
#include <iostream>

InputSystem::InputSystem(SDL_Window* window, double mouseSensitivity, SDL_bool capture) :
	mCurrentKeyState(),
	mPrevKeyState(),
	mWindow(window),
	mMouseState(0),
	mPrevMouseState(0),
	mScrollDir(0),
	mMouseX(0.0),
	mMouseY(0.0),
	mMouseSensitivity(mouseSensitivity),
	mMouseMode(capture == SDL_TRUE ? MouseMode::Relative : MouseMode::Absolute),
	mMouseCaptured(capture)
{

}

InputSystem::~InputSystem()
{
	std::cout << "Deleted Input System\n";
}

void InputSystem::StartFrame()
{
	// Save the last frame's keyboard state for leading edge detection
	std::memcpy(mPrevKeyState, mCurrentKeyState, sizeof(Uint8) * SDL_NUM_SCANCODES);

	// Save the last frame's mouse state
	mPrevMouseState = mMouseState;

	// Reset the scroll delta for new frame
	mScrollDir = 0;

	// Get the current snapshot of the keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// Copy that state into mCurrentKeyState
	std::memcpy(mCurrentKeyState, state, sizeof(Uint8) * SDL_NUM_SCANCODES);

	// Calculate mouse movement
	int x = 0;
	int y = 0;
	if (mMouseMode == MouseMode::Relative)
	{
		mMouseState = SDL_GetRelativeMouseState(&x, &y);
		mMouseX = x * mMouseSensitivity;
		mMouseY = -y * mMouseSensitivity;
	}
	else
	{
		mMouseState = SDL_GetMouseState(&x, &y);
		mMouseX = x;
		mMouseY = y;
	}
}

void InputSystem::EndFrame()
{
	// Empty for now
}

void InputSystem::HandleEvent(const SDL_Event& event)
{
	if (event.type == SDL_MOUSEWHEEL)
	{
		mScrollDir = event.wheel.y;
	}
}

bool InputSystem::IsKeyPressed(SDL_Scancode code) const
{
	return mCurrentKeyState[code] != 0;
}

bool InputSystem::IsKeyReleased(SDL_Scancode code) const
{
	return mCurrentKeyState[code] == 0;
}

bool InputSystem::IsKeyLeadingEdge(SDL_Scancode code) const
{
	return ((mCurrentKeyState[code] != 0) && (mPrevKeyState[code] == 0));
}

void InputSystem::ResetKeyboard() const
{
	SDL_ResetKeyboard();
}

bool InputSystem::IsMouseSingleClick(Uint8 button) const
{
	// Current frame click state for the button
	bool isClicked = (mMouseState & SDL_BUTTON(button)) != 0;

	// Previous frame click state for the button
	bool prevClick = (mPrevMouseState & SDL_BUTTON(button)) != 0;

	return isClicked && !prevClick;
}

bool InputSystem::IsMouseHeld(Uint8 button) const
{
	return ((mMouseState & SDL_BUTTON(button)) != 0);
}

bool InputSystem::IsMouseRelease(Uint8 button) const
{
	// Current frame click state for the button
	bool isClicked = (mMouseState & SDL_BUTTON(button)) != 0;

	// Previous frame click state for the button
	bool prevClick = (mPrevMouseState & SDL_BUTTON(button)) != 0;

	return !isClicked && prevClick;
}

void InputSystem::ToggleMouseCapture()
{
	if (mMouseCaptured == SDL_TRUE)
	{
		mMouseMode = MouseMode::Absolute;
		mMouseCaptured = SDL_FALSE;
	}
	else
	{
		mMouseMode = MouseMode::Relative;
		mMouseCaptured = SDL_TRUE;
	}

	mMouseX = 0;
	mMouseY = 0;

	CenterMouse();

	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(mMouseCaptured);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);
}

void InputSystem::CenterMouse()
{
	if (mWindow)
	{
		int windowW = 0;
		int windowH = 0;
		SDL_GetWindowSize(mWindow, &windowW, &windowH);
		SDL_WarpMouseInWindow(mWindow, windowW / 2, windowH / 2);
	}
}
