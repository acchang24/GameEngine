#include "Keyboard.h"
#include <iostream>

Keyboard::Keyboard() :
	mPrevKeyInputs()
{
}

Keyboard::~Keyboard()
{
	std::cout << "Deleted Keyboard\n";
}

const Uint8* Keyboard::GetState()
{
	return SDL_GetKeyboardState(NULL);
}

bool Keyboard::PressedLastFrame(SDL_Scancode code) const
{
	return mPrevKeyInputs[code];
}

bool Keyboard::HasLeadingEdge(const Uint8* state, SDL_Scancode code) const
{
	return (state[code] && !mPrevKeyInputs[code]);
}

void Keyboard::SavePrevKeyState(const Uint8* state, SDL_Scancode code)
{
	mPrevKeyInputs[code] = state[code];
}

void Keyboard::ResetKeyboard()
{
	SDL_ResetKeyboard();
}
