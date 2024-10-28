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
#include "Window/Backend/SDL2Window.h"
#include "Window/Backend/GLFWWindow.h"

namespace FIRE {

Window::Window(const WindowKind kind) FIRE_NOEXCEPT : kind(kind) {}

Uni<Window> Window::CreateSDL2(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT {
  return SDL2Window::Create(title, width, height);  
}

Uni<Window> Window::CreateGLFW(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT {
  return GLFWWindow::Create(title, width, height);  
}

void Window::SetEventCallback(const Function<void(Event&&)>& callback) FIRE_NOEXCEPT {
  event_callback = callback;
}

}
