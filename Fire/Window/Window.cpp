/**
  ******************************************************************************
  * @file           : Window.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Window.h"
#include "SDL2Window.h"

namespace FIRE {

Window::Window(const WindowKind kind) FIRE_NOEXCEPT : kind(kind) {}

Window* CreateSDL2Window(const char* title, const uint32_t width, const uint32_t height, const uint32_t flags) FIRE_NOEXCEPT {
  return SDL2Window::Create(title, width, height, flags);
}

void DestroySDL2Window(Window* window) FIRE_NOEXCEPT {
  SDL2Window::Destroy(static_cast<SDL2Window*>(window));
}

}
