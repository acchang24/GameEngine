#include "Console.h"
#include <deque>
#include <iostream>
#include "../Input/InputSystem.h"

Console::Console() :
	mInputBuffer(),
	mName("Console"),
	mIsVisible(false),
	mAutoScroll(true),
	mScrollToBottom(false)
{
}

Console::~Console()
{
	std::cout << "Deleted Console\n";
}

void Console::ProcessInput(InputSystem* input)
{
	if (input->IsKeyLeadingEdge(SDL_SCANCODE_GRAVE))
	{
		ToggleConsole();
	}
}

void Console::SetConsoleUI(const EngineContext& engineContext)
{
	Logger* logger = engineContext.logger;

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
			logger->AccessMessages([this](const std::deque<LogMessage>& messages)
			{
				for (const auto& msg : messages)
				{
					ImVec4 color = GetColorForLevel(msg.level);
					ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextUnformatted(msg.message.c_str());
					ImGui::PopStyleColor();
				}
			});

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
				ExecuteCommand(mInputBuffer, logger);
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

void Console::ExecuteCommand(const std::string& command, Logger* logger)
{
	LOG_PROMPT(command);

	if (command == "clear")
	{
		logger->Clear();
	}
	else if (command == "hi")
	{
		LOG_INFO("hi fucker");
	}
}

ImVec4 Console::GetColorForLevel(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Info:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	case LogLevel::Warning:
		return ImVec4(1.0f, 1.0f, 0.3f, 1.0f);
	case LogLevel::Error:
		return ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
	case LogLevel::Debug:
		return ImVec4(0.0f, 0.9f, 1.0f, 1.0f);
	case LogLevel::Prompt:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	default:
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}
