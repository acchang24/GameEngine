#include "Console.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <glm/glm.hpp>
#include "../Graphics/Renderer2D.h"
#include "../Graphics/Text.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

Console::Console(Logger* logger) :
	mInputBuffer(),
	mName("Console"),
	mLogger(logger),
	mIsVisible(false),
	mAutoScroll(true),
	mScrollToBottom(false)
{
}

Console::~Console()
{
	std::cout << "Deleted Console\n";
}

void Console::ProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse)
{
	if (keyboard->HasLeadingEdge(keyState, SDL_SCANCODE_GRAVE))
	{
		ToggleConsole();
	}

	keyboard->SavePrevKeyState(keyState, SDL_SCANCODE_GRAVE);
}

void Console::SetConsoleUI()
{
	if (mIsVisible)
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(mName.c_str(), &mIsVisible))
		{
			ImGui::End();
			return;
		}

		// Main scroll region
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true))
		{
			for (const LogMessage& msg : mLogger->GetMessages())
			{
				ImVec4 color = GetColorForLevel(msg.level);
				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(msg.message.c_str());
				ImGui::PopStyleColor();
			}

			if (mScrollToBottom || (mAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			{
				ImGui::SetScrollHereY(1.0f);
			}
			mScrollToBottom = false;
		}
		ImGui::EndChild();

		bool reclaim_focus = false;

		// Command line
		if (ImGui::InputText("Command", mInputBuffer, IM_ARRAYSIZE(mInputBuffer),
			ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (mInputBuffer[0])
			{
				ExecuteCommand(mInputBuffer);
				strcpy(mInputBuffer, "");
				mScrollToBottom = true;
				reclaim_focus = true;
			}
		}

		ImGui::SetItemDefaultFocus(); // Only necessary once when window opens

		if (reclaim_focus)
		{
			ImGui::SetKeyboardFocusHere(-1); // Set focus to previous widget (InputText)
		}

		ImGui::End();
	}
}

void Console::ExecuteCommand(const std::string& command)
{
	mLogger->Log("# " + command, LogLevel::Info);

	if (command == "clear")
	{
		mLogger->Clear();
	}
	else if (command == "hi")
	{
		mLogger->Log("hi fucker", LogLevel::Info);
	}
}

ImVec4 Console::GetColorForLevel(LogLevel level)
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

