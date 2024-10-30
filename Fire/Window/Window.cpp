/**
  ******************************************************************************
  * @file           : Window.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Window/Window.h"
#include "Window/Backend/SDL3Window.h"

namespace FIRE {

Uni<Window> Window::Create(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT {
  #ifdef FIRE_USE_SDL3_WINDOW
  return SDL3Window::Create(title, width, height);
  #else
  FIRE_CRITICAL("No Window Backend Specified!");
  FIRE_EXIT_FAILURE();
  #endif
}

}
