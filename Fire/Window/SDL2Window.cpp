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

namespace FIRE {

FireResult CreateSDL2(const uint32_t flags) {
  if (SDL_Init(flags) != 0) {
    
    return FIRE_FAILURE;
  }
  return FIRE_SUCCESS;
}

String& SDL2Window::GetTitle() {
}

uint32_t SDL2Window::GetWidth() {}

uint32_t SDL2Window::GetHeight() {}

void* SDL2Window::GetNativeWindow() {}


}