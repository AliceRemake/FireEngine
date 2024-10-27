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

// ReSharper disable once CppClassCanBeFinal
class FIRE_API Application {
public:
  virtual ~Application() FIRE_NOEXCEPT = default;
  virtual void Run() FIRE_NOEXCEPT;
};

}

#endif //FIRE_APPLICATION_H
