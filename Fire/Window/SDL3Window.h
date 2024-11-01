/**
  ******************************************************************************
  * @file           : SDL2Window.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_SDL3_WINDOW_H
#define FIRE_SDL3_WINDOW_H

#include "Common.h"

namespace FIRE {

class FIRE_API SDL3Window {
private:
  const char* title;
  uint32_t    width;
  uint32_t    height;
  SDL_Window* window;

public:
  SDL3Window(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  ~SDL3Window() FIRE_NOEXCEPT;
  
  // static Uni<SDL3Window> Create(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  
  FIRE_CONSTEXPR const char* GetTitle () const FIRE_NOEXCEPT { return title; }
  FIRE_CONSTEXPR uint32_t    GetWidth () const FIRE_NOEXCEPT { return width; }
  FIRE_CONSTEXPR uint32_t    GetHeight() const FIRE_NOEXCEPT { return height; }
  FIRE_CONSTEXPR SDL_Window* GetNative() const FIRE_NOEXCEPT { return window; }
};
  
}

#endif //FIRE_SDL3_WINDOW_H
