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
  virtual ~Window() FIRE_NOEXCEPT = default;
  virtual FIRE_CONSTEXPR const String& GetTitle() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR uint32_t GetWidth() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR uint32_t GetHeight() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR const void* GetNativeWindow() const FIRE_NOEXCEPT = 0;
};

Window* CreateSDL2Window(const char* title, uint32_t width, uint32_t height, uint32_t flags) FIRE_NOEXCEPT;

void DestroySDL2Window(Window* window) FIRE_NOEXCEPT;

}

#endif //FIRE_WINDOW_H
