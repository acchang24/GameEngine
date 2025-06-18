#pragma once
#include <SDL2/SDL_stdinc.h>
#include "../UI/EditorUI.h"
#include "../UI/imgui.h"
#include <string>
#include "Logger.h"

class Keyboard;
class Logger;
class Mouse;
class Renderer2D;
class Text;

struct TestConsole
{
	Logger* logger; // External message source
	char inputBuf[256];
	bool autoScroll = true;
	bool scrollToBottom = false;
	bool isVisible = false;

	TestConsole(Logger* logger) : logger(logger)
	{
		memset(inputBuf, 0, sizeof(inputBuf));
	}

	void ToggleConsole()
	{
		isVisible = !isVisible;
	}

	void Draw(const std::string& title)
	{
		if (isVisible)
		{
			//bool* test = &isVisible;
			ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin(title.c_str(), &isVisible))
			{
				ImGui::End();
				return;
			}

			// Main scroll region
			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true))
			{
				for (const LogMessage& msg : logger->GetMessages())
				{
					ImVec4 color = GetColorForLevel(msg.level);
					ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextUnformatted(msg.message.c_str());
					ImGui::PopStyleColor();
				}

				if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
					ImGui::SetScrollHereY(1.0f);
				scrollToBottom = false;
			}
			ImGui::EndChild();

			bool reclaim_focus = false;

			// Command line
			if (ImGui::InputText("Command", inputBuf, IM_ARRAYSIZE(inputBuf),
				ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (inputBuf[0])
				{
					ExecuteCommand(inputBuf);
					strcpy(inputBuf, "");
					scrollToBottom = true;
					reclaim_focus = true;
				}
			}

			ImGui::SetItemDefaultFocus(); // Only necessary once when window opens

			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(-1); // Set focus to previous widget (InputText)

			ImGui::End();
		}
		
	}

	void ExecuteCommand(const std::string& command)
	{
		logger->Log("# " + command, LogLevel::Info);

		if (command == "clear")
		{
			logger->Clear();
		}
		else if (command == "hi")
		{
			logger->Log("hi fucker", LogLevel::Info);
		}
	}

	ImVec4 GetColorForLevel(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::Info:    
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		case LogLevel::Warning: 
			return ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
		case LogLevel::Error:   
			return ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
		default:
			return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
};

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
