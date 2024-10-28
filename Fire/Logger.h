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

#include <spdlog/logger.h>
#include "Core.h"

namespace FIRE {

class FIRE_API Logger {
protected:
  static Ref<spdlog::logger> fire_logger;
  static Ref<spdlog::logger> client_logger;
  
public:
  static void Init();
  static Ref<spdlog::logger>& GetFireLogger();
  static Ref<spdlog::logger>& GetClientLogger();
};

}

#define FIRE_TRACE(...)    FIRE::Logger::GetFireLogger()->trace(__VA_ARGS__)
#define FIRE_DEBUG(...)    FIRE::Logger::GetFireLogger()->debug(__VA_ARGS__)
#define FIRE_INFO(...)     FIRE::Logger::GetFireLogger()->info(__VA_ARGS__)
#define FIRE_WARN(...)     FIRE::Logger::GetFireLogger()->warn(__VA_ARGS__)
#define FIRE_ERROR(...)    FIRE::Logger::GetFireLogger()->error(__VA_ARGS__)
#define FIRE_CRITICAL(...) FIRE::Logger::GetFireLogger()->critical(__VA_ARGS__)

#define CLIENT_TRACE(...)    FIRE::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CLIENT_DEBUG(...)    FIRE::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define CLIENT_INFO(...)     FIRE::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_WARN(...)     FIRE::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_ERROR(...)    FIRE::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_CRITICAL(...) FIRE::Logger::GetClientLogger()->critical(__VA_ARGS__)

#endif //FIRE_LOGGER_H
