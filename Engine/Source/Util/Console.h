#pragma once
#include <string>
#include "../EngineUI/imgui.h"
#include "../EngineContext.h"
#include "Logger.h"

class InputSystem;

class Console
{
public:
	Console();
	~Console();

	// Process inputs for console
	// @param - InputSystem* for the input system
	void ProcessInput(InputSystem* input);

	// Sets the console window and all of its components
	// @param - const EngineContext& for the engine context
	void SetConsoleUI(const EngineContext& engineContext);

	// Executes a console command
	// @param - const std::string& for the command
	// @param - Logger* for the engine's logger
	void ExecuteCommand(const std::string& command, Logger* logger);

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

	// Toggle if console is visible
	bool mIsVisible;

	// Is auto scroll
	bool mAutoScroll;

	// Is scroll to bottom
	bool mScrollToBottom;
};
