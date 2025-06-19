#include "Logger.h"
#include <iostream>

Logger::Logger(size_t maxMessages):
	mMaxMessages(maxMessages)
{
}

Logger::~Logger()
{
	std::cout << "Deleted Logger\n";

	mMessages.clear();
}

Logger* Logger::Get()
{
	static Logger s_Logger;

	return &s_Logger;
}

void Logger::Log(const std::string& message, LogLevel level)
{
	if (mMessages.size() >= mMaxMessages)
	{
		// Remove the front (oldest message)
		mMessages.pop_front();
	}
	mMessages.push_back({ message, level });
}
