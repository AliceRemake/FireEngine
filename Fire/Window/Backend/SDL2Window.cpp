/**
  ******************************************************************************
  * @file           : SDL2Window.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Window/Backend/SDL2Window.h"
#include "Event/WindowEvent.h"
#include "Logger.h"

namespace FIRE {

static uint32_t SDL2_WINDOW_COUNT = 0;

SDL2Window::SDL2Window(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT
: Window(FIRE_WINDOW_KIND_SDL2), title(title), width(width), height(height), window(nullptr) {
  FIRE_ASSERT(SDL2_WINDOW_COUNT < UINT32_MAX);
  
  if(++SDL2_WINDOW_COUNT == 1) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      FIRE_CRITICAL("Can Not Create SDL2!\n");
      FIRE_ASSERT(0);
    }
  }

  // TODO
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
    SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
  );

  if (window == nullptr) {
    FIRE_ERROR("Can Not Create SDL2 Window!\n");
    FIRE_ASSERT(0);
  }
}

SDL2Window::~SDL2Window() FIRE_NOEXCEPT {
  FIRE_ASSERT(SDL2_WINDOW_COUNT > 0);

  SDL_DestroyWindow(window);
  if (--SDL2_WINDOW_COUNT == 0) {
    SDL_Quit();    
  }
}

void SDL2Window::PollEvent() const FIRE_NOEXCEPT {
  SDL_Event event;
  SDL_PollEvent(&event);

  if (event.type == SDL_QUIT) {
    if (event_callback != nullptr) {
      event_callback(WindowCloseEvent());
    }
  }
  
  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
    if (event_callback != nullptr) {
      event_callback(WindowCloseEvent());
    }
  }

  // TODO
}

Uni<SDL2Window> SDL2Window::Create(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT {
  return CreateUni<SDL2Window>(title, width, height);
}

}