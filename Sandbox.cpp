/**
  ******************************************************************************
  * @file           : sandbox.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */


#include "Fire.h"

int main(int argc, char** argv) {

  FIRE::Logger::Init();
  
  FIRE::Logger::GetFireLogger()->trace("trace");
  FIRE::Logger::GetFireLogger()->debug("debug");
  FIRE::Logger::GetFireLogger()->info("info");
  FIRE::Logger::GetFireLogger()->warn("warn");
  FIRE::Logger::GetFireLogger()->error("error");
  FIRE::Logger::GetFireLogger()->critical("critical");

  FIRE::Logger::GetClientLogger()->trace("trace");
  FIRE::Logger::GetClientLogger()->debug("debug");
  FIRE::Logger::GetClientLogger()->info("info");
  FIRE::Logger::GetClientLogger()->warn("warn");
  FIRE::Logger::GetClientLogger()->error("error");
  FIRE::Logger::GetClientLogger()->critical("critical");
  
  return 0;
}
