#pragma once

#include "Logger.h"

#ifdef NDEBUG
	// Release build logging
	#define LOG_DEBUG(msg)    ((void)0)
#else
	// Debug builds logging
	#define LOG_DEBUG(msg)    Logger::Get()->Log("[DEBUG] " + std::string(msg), LogLevel::Debug)
#endif

#define LOG_INFO(msg)     Logger::Get()->Log("[INFO] " + std::string(msg), LogLevel::Info)
#define LOG_WARNING(msg)     Logger::Get()->Log("[WARNING] " + std::string(msg), LogLevel::Warning)
#define LOG_ERROR(msg)    Logger::Get()->Log("[ERROR] " + std::string(msg), LogLevel::Error)