/**
  ******************************************************************************
  * @file           : Window.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_WINDOW_H
#define FIRE_WINDOW_H

#include "Core.h"

namespace FIRE {

class Window {
public:
  virtual ~Window() = default;
  virtual String& GetTitle() = 0;
  virtual uint32_t GetWidth() = 0;
  virtual uint32_t GetHeight() = 0;
  virtual void* GetNativeWindow() = 0;
};

}

#endif //FIRE_WINDOW_H
