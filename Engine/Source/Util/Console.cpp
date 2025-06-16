#include "Console.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <glm/glm.hpp>
#include "../Graphics/Renderer2D.h"
#include "../Graphics/Text.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"
#include "Logger.h"

Console::Console(Logger* logger) :
	mLogger(logger),
	mScrollOffset(0),
	mMaxVisibleLines(20),
	mLineHeight(16.0f),
	mConsoleHeight(mLineHeight * mMaxVisibleLines),
	mIsVisible(false)
{
}

Console::~Console()
{
	std::cout << "Deleted Console\n";
}

void Console::ProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse)
{
	if (!mIsVisible)
	{
		return;
	}

	// Scroll based on arrow key or mouse
	if (keyboard->HasLeadingEdge(keyState, SDL_SCANCODE_UP) || mouse->GetScrollDir() >= 1)
	{
		mScrollOffset += 2;
	}
	if (keyboard->HasLeadingEdge(keyState, SDL_SCANCODE_DOWN) || mouse->GetScrollDir() <= -1)
	{
		mScrollOffset -= 2;
	}

	// Clamp scroll offset
	int maxScroll = std::max(0, static_cast<int>(mLogger->GetNumMessages()) - mMaxVisibleLines);
	mScrollOffset = std::clamp(mScrollOffset, 0, maxScroll);

	keyboard->SavePrevKeyState(keyState, SDL_SCANCODE_UP);
	keyboard->SavePrevKeyState(keyState, SDL_SCANCODE_DOWN);
}

void Console::Render(Renderer2D* renderer, Text* textRenderer)
{
	if (!mIsVisible)
	{
		return;
	}

	float consoleWidth = renderer->GetWidth() * 0.25f;
	float consoleHeight = mMaxVisibleLines * mLineHeight + 32;

	float consoleY = 0.0f;

	// Title height 32
	renderer->DrawRect(0.0, consoleY, consoleWidth, 32.0f, glm::vec3(0.175f, 0.175f, 0.175f));
	// Padding 6.0f
	renderer->DrawText("Debug Console", 6.0f, consoleY + 6.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// Exit button (top-right)
	float buttonW = 24.0f;
	float buttonH = 20.0f;
	float buttonX = consoleWidth - buttonW - 6.0f;
	float buttonY = consoleY + (32.0f - buttonH) / 2.0f;
	renderer->DrawText("X", buttonX + 6.0f, buttonY + 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	float logY = consoleY + 32;
	float logHeight = consoleHeight + 56;
	renderer->DrawRect(0, logY, consoleWidth, logHeight, glm::vec3(0.175f, 0.175f, 0.175f));

	const std::deque<LogMessage>& messages = mLogger->GetMessages();

	// Get the start and end message indices based on visible lines and offset
	int startIndex = std::max(0, static_cast<int>(messages.size() - mMaxVisibleLines) - mScrollOffset);
	int endIndex = std::min(startIndex + mMaxVisibleLines, static_cast<int>(messages.size()));

	float y = logY + 6.0f;
	for (int i = startIndex; i < endIndex; ++i)
	{
		const LogMessage& m = messages[i];

		glm::vec3 color(1.0f);

		switch (m.level)
		{
		case LogLevel::Warning:
			color = glm::vec3(1.0f, 1.0f, 0.0f);
			break;
		case LogLevel::Error:
			color = glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		}

		textRenderer->RenderText(m.message, 6.0f, y, 1.0f, color);

		y += mLineHeight + 4;
	}

	// --- Input text box ---
	float inputY = logY + logHeight;
	renderer->DrawRect(0, inputY, consoleWidth, 24.0f, glm::vec3(0.175f, 0.175f, 0.175f));
	renderer->DrawText("> ", 6.0f, inputY + 4.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}
