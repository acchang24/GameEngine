#include "Logger.h"
#include <iostream>

Logger::Logger(size_t maxMessages):
	mMaxMessages(maxMessages)
{
}

Logger::~Logger()
{
	std::cout << "Deleted Logger\n";
}

void Logger::Log(const std::string& message, LogLevel level)
{
	if (mMaxMessages >= mMessages.size())
	{
		// Remove the front (oldest message)
		mMessages.pop_front();
	}
	mMessages.push_back({ message, level });
}
