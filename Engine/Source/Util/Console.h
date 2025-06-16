#pragma once
#include <SDL2/SDL_stdinc.h>

class Keyboard;
class Logger;
class Mouse;
class Renderer2D;
class Text;

class Console
{
public:
	// Console constructor:
	// @param - Logger* for messages
	Console(Logger* logger);
	~Console();

	// Process inputs for console
	void ProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse);

	// Renders the console
	// @param - Renderer2D* to render console box
	// @param - Text* for the text renderer
	void Render(Renderer2D* renderer, Text* textRenderer);

	// Toggles the visibility of the console
	void ToggleConsole() { mIsVisible = !mIsVisible; };

	// Returns true if the console is visible, false if not
	// @return - bool for console visibility
	bool IsVisible() const { return mIsVisible; }

private:
	// Logger to keep track of messages in the console
	Logger* mLogger;

	// Scroll offset
	int mScrollOffset;

	// Maximum number of lines visible in the console
	int mMaxVisibleLines;

	// Height of line in pixels
	float mLineHeight;

	// Console height in pixels
	float mConsoleHeight;

	// Toggle if console is visible
	bool mIsVisible;
};
