/**
  ******************************************************************************
  * @file           : SDL2Window.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Window/SDL3Window.h"

namespace FIRE {

// Uni<SDL3Window> SDL3Window::Create(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT {
//   return CreateUni<SDL3Window>(title, width, height);
// }
//   
SDL3Window::SDL3Window(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT
: title(title), width(width), height(height), window(nullptr) {

  window = SDL_CreateWindow(title, width, height,
     SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN | SDL_WINDOW_HIGH_PIXEL_DENSITY
     // | SDL_WINDOW_HIDDEN
  );
  
  if (window == nullptr) {
    FIRE_CRITICAL("Can Not Create SDL2 Window!\n");
    FIRE_EXIT_FAILURE();
  }
}

SDL3Window::~SDL3Window() FIRE_NOEXCEPT {
  SDL_DestroyWindow(window);
}

}
