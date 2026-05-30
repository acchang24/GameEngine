#include "Keyboard.h"
#include <iostream>

Keyboard::Keyboard() :
	mCurrentKeyState(),
	mPrevKeyState()
{
}

Keyboard::~Keyboard()
{
	std::cout << "Deleted Keyboard\n";
}
