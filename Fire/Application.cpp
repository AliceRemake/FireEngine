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

const Window& Application::GetWindow() const FIRE_NOEXCEPT {
  FIRE_ASSERT(window != nullptr);
  return *window;
}

}