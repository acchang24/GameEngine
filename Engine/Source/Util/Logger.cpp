#include "Logger.h"
#include <iostream>

Logger::Logger()
{
}

Logger::~Logger()
{
	std::cout << "Deleted Logger\n";
}

void Logger::Log(const std::string& message, LogLevel level)
{
	LogMessage newMessage = { message, level };
	mMessages.emplace_back(newMessage);
}
