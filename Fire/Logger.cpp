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

std::shared_ptr<spdlog::logger> FireLogger;
std::shared_ptr<spdlog::logger> ClientLogger;

void Logger::Init()
{
  spdlog::set_pattern("%^[%T] %n: %v%$"); 
  FireLogger = spdlog::stdout_color_mt("FIRE");
  FireLogger->set_level(spdlog::level::trace);
  ClientLogger = spdlog::stdout_color_mt("CLIENT");
  ClientLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::GetFireLogger() { return FireLogger; }

std::shared_ptr<spdlog::logger>& Logger::GetClientLogger() { return ClientLogger; }

}
