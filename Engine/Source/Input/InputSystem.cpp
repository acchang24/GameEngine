#include "InputSystem.h"
#include <cstring>
#include <iostream>

InputSystem::InputSystem() :
	mKeyboard(),
	mMouse(),
	mWindow(nullptr)
{

}

InputSystem::~InputSystem()
{
	std::cout << "Deleted Input System\n";
}

bool InputSystem::Init(SDL_Window* window, double mouseSensitivity, SDL_bool capture)
{
	// Check if window exists. If it doesn't exist, fail and return false
	if (!window)
	{
		return false;
	}

	mWindow = window;

	mMouse.SetSensitivity(mouseSensitivity);
	mMouse.SetCapture(capture);
	mMouse.SetMode(capture == SDL_TRUE ? MouseMode::Relative : MouseMode::Absolute);

	return true;
}

void InputSystem::StartFrame()
{
	mKeyboard.SavePrevKeyState();

	mMouse.ResetState();
}

void InputSystem::GetCurrentState()
{
	// Get the current snapshot of the keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// Copy that state into mCurrentKeyState
	mKeyboard.SaveCurrentKeyState(state);

	mMouse.CalculateMovement();
}

void InputSystem::EndFrame()
{
	// Empty for now
}

void InputSystem::HandleEvent(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mMouse.SetScrollDir(event.wheel.y);
		break;
	};
}
