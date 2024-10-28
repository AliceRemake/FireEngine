/**
  ******************************************************************************
  * @file           : SDL2Window.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_SDL2WINDOW_H
#define FIRE_SDL2WINDOW_H

#include <SDL.h>
#include "Window/Window.h"

namespace FIRE {

class SDL2Window final : public Window {
public:
  static FIRE_CONSTEXPR bool ClassOf(const Window* ptr) FIRE_NOEXCEPT {
    return ptr->GetKind() == FIRE_WINDOW_KIND_SDL2;
  }
  
private:
  String title;
  uint32_t width;
  uint32_t height;
  SDL_Window* window;

public:
  SDL2Window(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  ~SDL2Window() FIRE_NOEXCEPT FIRE_OVERRIDE;
  static Uni<SDL2Window> Create(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;

public:
  FIRE_CONSTEXPR const String& GetTitle() const FIRE_NOEXCEPT FIRE_OVERRIDE { return title; }
  FIRE_CONSTEXPR uint32_t GetWidth() const FIRE_NOEXCEPT FIRE_OVERRIDE { return width; }
  FIRE_CONSTEXPR uint32_t GetHeight() const FIRE_NOEXCEPT FIRE_OVERRIDE { return height; }
  FIRE_CONSTEXPR const void* GetNativeWindow() const FIRE_NOEXCEPT FIRE_OVERRIDE { return window; }
  void PollEvent() const FIRE_NOEXCEPT FIRE_OVERRIDE;
};

}

#endif //FIRE_SDL2WINDOW_H
