/**
  ******************************************************************************
  * @file           : Logger.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef LOGGER_H
#define LOGGER_H

#include <spdlog/logger.h>
#include "Core.h"

namespace FIRE {

class FIRE_API Logger {
public:
  static void Init();
  static std::shared_ptr<spdlog::logger>& GetFireLogger();
  static std::shared_ptr<spdlog::logger>& GetClientLogger();
  
};

}

#endif //LOGGER_H
