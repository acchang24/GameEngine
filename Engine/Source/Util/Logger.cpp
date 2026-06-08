#include "Logger.h"
#include <iostream>

Logger::Logger(size_t maxMessages):
	mMaxMessages(maxMessages)
{
}

Logger::~Logger()
{
	std::cout << "Deleted Logger\n";

	Clear();
}

void Logger::Log(const std::string& message, LogLevel level)
{
	// Lock the mutex to protect the deque
	std::lock_guard<std::mutex> lock(mMutex);

	if (mMessages.size() >= mMaxMessages)
	{
		// Remove the front (oldest message)
		mMessages.pop_front();
	}
	mMessages.push_back({ message, level });
}

size_t Logger::GetNumMessages() const
{
	// Lock mutex
	std::lock_guard<std::mutex> lock(mMutex);

	return mMessages.size();
}

void Logger::AccessMessages(const std::function<void(const std::deque<LogMessage>&)>& getMessage) const 
{
	// Lock mutex
	std::lock_guard<std::mutex> lock(mMutex);

	// Pass mMessages into function
	getMessage(mMessages);
}

void Logger::Clear()
{
	// Lock mutex
	std::lock_guard<std::mutex> lock(mMutex);

	mMessages.clear();
}
