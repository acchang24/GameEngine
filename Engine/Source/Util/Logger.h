#pragma once
#include <deque>
#include <string>
#include <mutex>

enum class LogLevel
{
	Info,		// Message that logs info
	Warning,	// Message that logs a warning
	Error,		// Message that logs an error
	Debug,		// Message that logs a debug
};

struct LogMessage
{
	std::string message;
	LogLevel level;
};

class Logger
{
public:
	// Logger constructor:
	// @param - size_t for the max number of messages a logger can hold (defaults to 1000)
	Logger(size_t maxMessages = 1000);
	~Logger();

	// Logs a message and its level by adding it to the deque of LogMessages
	// @param - const std::string& for the message
	// @param - LogLevel for the message's level
	void Log(const std::string& message, LogLevel level);

	// Gets the number of messages from the logger
	// @return - size_t for the number of messages
	size_t GetNumMessages() const;

	// Gets the logger messages
	// @return - const std::deque<LogMessage>& for the messages
	const std::deque<LogMessage>& GetMessages() const { return mMessages; }

	// Removes all log messages from the deque
	void Clear();

private:
	// Deque of messages
	std::deque<LogMessage> mMessages;

	// Maximum number of messages
	size_t mMaxMessages;

	// Mutex to protect deque across threads
	mutable std::mutex mMutex;
};


// GLOBAL MACRO BRIDGE SYSTEM
namespace Log
{
	inline Logger* ActiveLogger = nullptr;
}

// LOGGER MACROS
#ifdef NDEBUG
// Release build logging
#define LOG_DEBUG(msg)    ((void)0)
#else
// Debug builds logging
#define LOG_DEBUG(msg)    if(Log::ActiveLogger) Log::ActiveLogger->Log("[DEBUG] " + std::string(msg), LogLevel::Debug)
#endif

#define LOG_INFO(msg)     if(Log::ActiveLogger) Log::ActiveLogger->Log("[INFO] " + std::string(msg), LogLevel::Info)
#define LOG_WARNING(msg)     if(Log::ActiveLogger) Log::ActiveLogger->Log("[WARNING] " + std::string(msg), LogLevel::Warning)
#define LOG_ERROR(msg)    if(Log::ActiveLogger) Log::ActiveLogger->Log("[ERROR] " + std::string(msg), LogLevel::Error)
