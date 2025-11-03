#pragma once 

#include <spdlog/spdlog.h>

namespace Engine {

#ifdef NDEBUG

#define LOG_INFO(...)	
#define LOG_WARN(...)	
#define LOG_ERR(...)	
#define LOG_CRIT(...)	

#else

#define LOG_INFO(...)	spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)	spdlog::warn("[{}:{}]" __VA_ARGS__, __FILE__, __LINE__)
#define LOG_ERR(...)	spdlog::error("[{}:{}]" __VA_ARGS__, __FILE__, __LINE__)
#define LOG_CRIT(...)	spdlog::critical("[{}:{}]" __VA_ARGS__, __FILE__, __LINE__)

#endif

}