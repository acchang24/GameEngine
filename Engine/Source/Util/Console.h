#pragma once
#include <string>
#include <SDL2/SDL_stdinc.h>
#include "../EngineUI/imgui.h"
#include "Logger.h"

class Keyboard;
class Mouse;

class Console
{
public:
	// Console constructor:
	// @param - Logger* for messages
	Console(Logger* logger);
	~Console();

	// Process inputs for console
	void ProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse);

	// Sets the console window and all of its components
	void SetConsoleUI();

	// Executes a console command
	// @param - const std::string& for the command
	void ExecuteCommand(const std::string& command);

	// Gets a color for ImGUI text
	// @param - LogLevel for the log message's level
	// @return - ImVec4 for the ImGUI color
	ImVec4 GetColorForLevel(LogLevel level);

	// Toggles the visibility of the console
	void ToggleConsole() { mIsVisible = !mIsVisible; };

	// Returns true if the console is visible, false if not
	// @return - bool for console visibility
	bool IsVisible() const { return mIsVisible; }

private:
	// Char input buffer for entering commands
	char mInputBuffer[256];

	// Name of console window
	std::string mName;

	// Logger to keep track of messages in the console
	Logger* mLogger;

	// Toggle if console is visible
	bool mIsVisible;

	// Is auto scroll
	bool mAutoScroll;

	// Is scroll to bottom
	bool mScrollToBottom;
};
