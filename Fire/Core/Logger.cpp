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
  return fire_logger;
}

Ref<spdlog::logger>& Logger::GetClientLogger() {
  static Ref<spdlog::logger> client_logger
    = spdlog::stdout_color_mt("CLIENT");
  return client_logger;
}

}
