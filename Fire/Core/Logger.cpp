/**
  ******************************************************************************
  * @file           : Logger.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FIRE {

void Logger::SetPattern(const String& pattern) {
  spdlog::set_pattern(pattern);
}

Ref<spdlog::logger>& Logger::GetFireLogger() {
  static Ref<spdlog::logger> fire_logger
    = spdlog::stdout_color_mt("FIRE");
  fire_logger->set_level(spdlog::level::level_enum::trace);
  return fire_logger;
}

Ref<spdlog::logger>& Logger::GetClientLogger() {
  static Ref<spdlog::logger> client_logger
    = spdlog::stdout_color_mt("CLIENT");
  client_logger->set_level(spdlog::level::level_enum::trace);
  return client_logger;
}

}
