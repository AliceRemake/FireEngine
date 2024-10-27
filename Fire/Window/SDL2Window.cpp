/**
  ******************************************************************************
  * @file           : SDL2Window.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "SDL2Window.h"
#include "Logger.h"

namespace FIRE {

FireResult CreateSDL2(const uint32_t flags) FIRE_NOEXCEPT {
  if (SDL_Init(flags) != 0) {
    FIRE_ERROR("Can Not Create SDL2!\n");
    return FIRE_FAILURE;
  }
  return FIRE_SUCCESS;
}

void DestroySDL2() FIRE_NOEXCEPT {
  SDL_Quit();
}

SDL2Window* SDL2Window::Create(const char* title, const uint32_t width, const uint32_t height, const uint32_t flags) FIRE_NOEXCEPT {
  SDL2Window* sdl2_window = new SDL2Window;
  sdl2_window->title = title;
  sdl2_window->width = width;
  sdl2_window->height = height;
  sdl2_window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (sdl2_window->window == nullptr) {
    FIRE_ERROR("Can Not Create SDL2 Window!\n");
    return sdl2_window;
  }
  return sdl2_window;
}

void SDL2Window::Destroy(const SDL2Window* sdl2_window) FIRE_NOEXCEPT {
  if (sdl2_window == nullptr) return;
  SDL_DestroyWindow(sdl2_window->window);
}

FIRE_CONSTEXPR const String& SDL2Window::GetTitle() const FIRE_NOEXCEPT {
  return title;
}

FIRE_CONSTEXPR uint32_t SDL2Window::GetWidth() const FIRE_NOEXCEPT {
  return width;
}

FIRE_CONSTEXPR uint32_t SDL2Window::GetHeight() const FIRE_NOEXCEPT {
  return height;
}

FIRE_CONSTEXPR const void* SDL2Window::GetNativeWindow() const FIRE_NOEXCEPT {
  return window;
}

}