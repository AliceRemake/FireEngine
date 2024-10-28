/**
  ******************************************************************************
  * @file           : Logger.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Logger.h"

namespace FIRE {

Ref<spdlog::logger> Logger::fire_logger = nullptr;
Ref<spdlog::logger> Logger::client_logger = nullptr;

void Logger::Init()
{
  spdlog::set_pattern("%^[%T] %n: %v%$"); 
  fire_logger = spdlog::stdout_color_mt("FIRE");
  fire_logger->set_level(spdlog::level::trace);
  client_logger = spdlog::stdout_color_mt("CLIENT");
  client_logger->set_level(spdlog::level::trace);
}

Ref<spdlog::logger>& Logger::GetFireLogger() { return fire_logger; }

Ref<spdlog::logger>& Logger::GetClientLogger() { return client_logger; }

}
