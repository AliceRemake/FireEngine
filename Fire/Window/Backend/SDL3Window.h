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

#include "Window/Window.h"

namespace FIRE {

#ifdef FIRE_USE_SDL3_WINDOW
  
class SDL3Window final : public Window {
  FIRE_RTTI_DERIVE(Window, FIRE_WINDOW_KIND_SDL3);

public:
  FIRE_CONSTEXPR const String& GetTitle() const FIRE_NOEXCEPT FIRE_OVERRIDE;
  FIRE_CONSTEXPR uint32_t GetWidth() const FIRE_NOEXCEPT FIRE_OVERRIDE;
  FIRE_CONSTEXPR uint32_t GetHeight() const FIRE_NOEXCEPT FIRE_OVERRIDE;
  FIRE_CONSTEXPR void* GetNative() const FIRE_NOEXCEPT FIRE_OVERRIDE;
  Ref<Event> PollEvent() const FIRE_NOEXCEPT FIRE_OVERRIDE;
  static Uni<SDL3Window> Create(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  
private:
  String title;
  uint32_t width;
  uint32_t height;
  SDL_Window* window;

public:
  SDL3Window(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  ~SDL3Window() FIRE_NOEXCEPT FIRE_OVERRIDE;
  
};

#endif
  
}

#endif //FIRE_SDL2WINDOW_H
