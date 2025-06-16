#include "Console.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <glm/glm.hpp>
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

void Console::Render(Text* textRenderer)
{
	if (!mIsVisible)
	{
		return;
	}

	// TODO: Draw rectangle here


	const std::deque<LogMessage>& messages = mLogger->GetMessages();

	// Get the start and end message indices based on visible lines and offset
	int startIndex = std::max(0, static_cast<int>(messages.size() - mMaxVisibleLines) - mScrollOffset);
	int endIndex = std::min(startIndex + mMaxVisibleLines, static_cast<int>(messages.size()));

	float y = 10.0f;
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

		// TODO: Draw text here: Add const x offset???
		textRenderer->RenderText(m.message, 10.0f, y, 1.0f, color);

		y += mLineHeight + 4;
	}
}
