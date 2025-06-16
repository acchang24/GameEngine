#pragma once
#include <string>
#include <deque>

enum class LogLevel
{
	Info,		// Message that logs info
	Warning,	// Message that logs a warning
	Error		// Message that logs an error
};

struct LogMessage
{
	std::string message;
	LogLevel level;
};

class Logger
{
public:
	Logger(size_t maxMessages = 1000);
	~Logger();

	// Logs a message and its level by adding it to the deque of LogMessages
	// @param - const std::string& for the message
	// @param - LogLevel for the message's level
	void Log(const std::string& message, LogLevel level);

	// Gets the number of messages from the logger
	// @return - size_t for the number of messages
	size_t GetNumMessages() const { return mMessages.size(); }

	// Gets the logger messages
	// @return - const std::deque<LogMessage>& for the messages
	const std::deque<LogMessage>& GetMessages() const { return mMessages; }

private:
	// Deque of messages
	std::deque<LogMessage> mMessages;

	// Maximum number of messages
	size_t mMaxMessages;
};
