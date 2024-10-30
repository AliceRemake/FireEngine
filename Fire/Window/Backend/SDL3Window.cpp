/**
  ******************************************************************************
  * @file           : SDL2Window.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Event/WindowEvent.h"
#include "Window/Backend/SDL3Window.h"

namespace FIRE {

#ifdef FIRE_USE_SDL3_WINDOW

FIRE_CONSTEXPR const String& SDL3Window::GetTitle() const FIRE_NOEXCEPT { return title; }

FIRE_CONSTEXPR uint32_t SDL3Window::GetWidth() const FIRE_NOEXCEPT { return width; }

FIRE_CONSTEXPR uint32_t SDL3Window::GetHeight() const FIRE_NOEXCEPT { return height; }

FIRE_CONSTEXPR void* SDL3Window::GetNative() const FIRE_NOEXCEPT { return window; }

Ref<Event> SDL3Window::PollEvent() const FIRE_NOEXCEPT {
  SDL_Event event;
  SDL_PollEvent(&event);

  if (event.type == SDL_EVENT_QUIT) {
    return CreateRef<WindowCloseEvent>();
  }

  if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)) {
    return CreateRef<WindowCloseEvent>();
  }

  return nullptr;
}

Uni<SDL3Window> SDL3Window::Create(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT {
  return CreateUni<SDL3Window>(title, width, height);
}
  
SDL3Window::SDL3Window(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT
: Window(FIRE_WINDOW_KIND_SDL3), title(title), width(width), height(height), window(nullptr) {

  #ifdef FIRE_USE_VULKAN_HRI
  window = SDL_CreateWindow(title, width, height,
     SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN | SDL_WINDOW_HIGH_PIXEL_DENSITY
     // | SDL_WINDOW_HIDDEN
  );
  #else
  FIRE_CRITICAL("No Graphics API Specified!");
  FIRE_EXIT_FAILURE();
  #endif
  
  if (window == nullptr) {
    FIRE_CRITICAL("Can Not Create SDL2 Window!\n");
    FIRE_EXIT_FAILURE();
  }
}

SDL3Window::~SDL3Window() FIRE_NOEXCEPT {
  SDL_DestroyWindow(window);
}

#endif

}