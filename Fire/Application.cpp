/**
  ******************************************************************************
  * @file           : Application.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Application.h"

namespace FIRE {

Application* Application::CreateApplication() { return new Application; }
  
void Application::Run() { while(true); }

}
