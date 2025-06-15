#pragma once
#include <string>
#include <vector>

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
	Logger();
	~Logger();

	// Logs a message and its level by adding it to the vector of LogMessages
	// @param - const std::string& for the message
	// @param - LogLevel for the message's level
	void Log(const std::string& message, LogLevel level);

	// Gets the logger messages
	// @return - const std::vector<LogMessage>& for the messages
	const std::vector<LogMessage>& GetMessages() const { return mMessages; }

private:
	std::vector<LogMessage> mMessages;
};
