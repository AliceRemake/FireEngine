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

std::shared_ptr<spdlog::logger> fire_logger;
std::shared_ptr<spdlog::logger> client_logger;

void Logger::Init()
{
  spdlog::set_pattern("%^[%T] %n: %v%$"); 
  fire_logger = spdlog::stdout_color_mt("FIRE");
  fire_logger->set_level(spdlog::level::trace);
  client_logger = spdlog::stdout_color_mt("CLIENT");
  client_logger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::GetFireLogger() { return fire_logger; }

std::shared_ptr<spdlog::logger>& Logger::GetClientLogger() { return client_logger; }

}
