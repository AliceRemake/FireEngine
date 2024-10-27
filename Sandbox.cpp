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
  
  FIRE_TRACE("trace");
  FIRE_DEBUG("debug");
  FIRE_INFO("info");
  FIRE_WARN("warn");
  FIRE_ERROR("error");
  FIRE_CRITICAL("critical");
  
  CLIENT_TRACE("trace");
  CLIENT_DEBUG("debug");
  CLIENT_INFO("info");
  CLIENT_WARN("warn");
  CLIENT_ERROR("error");
  CLIENT_CRITICAL("critical");

  FIRE::Application* app = FIRE::Application::CreateApplication();
  app->Run();
  delete app;
  
  return 0;
}
