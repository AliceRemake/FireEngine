/**
  ******************************************************************************
  * @file           : Application.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_APPLICATION_H
#define FIRE_APPLICATION_H

#include "Core.h"

namespace FIRE {

class FIRE_API Application {

public:
  static Application* CreateApplication();
  
  FIRE_NORETURN void Run();

};

}

#endif //FIRE_APPLICATION_H
