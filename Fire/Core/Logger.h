/**
  ******************************************************************************
  * @file           : Logger.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_LOGGER_H
#define FIRE_LOGGER_H

#include "Common.h"
#include <spdlog/logger.h>

namespace FIRE {

class FIRE_API Logger {
public:
  static void SetPattern(const String& pattern);
  static Ref<spdlog::logger>& GetFireLogger();
  static Ref<spdlog::logger>& GetClientLogger();
};

#define FIRE_TRACE(...)    Logger::GetFireLogger()->trace(__VA_ARGS__)
#define FIRE_DEBUG(...)    Logger::GetFireLogger()->debug(__VA_ARGS__)
#define FIRE_INFO(...)     Logger::GetFireLogger()->info(__VA_ARGS__)
#define FIRE_WARN(...)     Logger::GetFireLogger()->warn(__VA_ARGS__)
#define FIRE_ERROR(...)    Logger::GetFireLogger()->error(__VA_ARGS__)
#define FIRE_CRITICAL(...) Logger::GetFireLogger()->critical(__VA_ARGS__)

}

#define CLIENT_TRACE(...)    FIRE::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CLIENT_DEBUG(...)    FIRE::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define CLIENT_INFO(...)     FIRE::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_WARN(...)     FIRE::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_ERROR(...)    FIRE::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_CRITICAL(...) FIRE::Logger::GetClientLogger()->critical(__VA_ARGS__)

#endif //FIRE_LOGGER_H
