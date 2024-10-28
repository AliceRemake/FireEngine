/**
  ******************************************************************************
  * @file           : GLFWWindow.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_GLFW_WINDOW_H
#define FIRE_GLFW_WINDOW_H

#include <GLFW/glfw3.h>
#include "Window/Window.h"

namespace FIRE {

class GLFWWindow final : public Window {
public:
  static FIRE_CONSTEXPR bool ClassOf(const Window* ptr) FIRE_NOEXCEPT {
    return ptr->GetKind() == FIRE_WINDOW_KIND_GLFW;
  }
  
private:
  String title;
  uint32_t width;
  uint32_t height;
  GLFWwindow* window;

public:
  GLFWWindow(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  ~GLFWWindow() FIRE_NOEXCEPT FIRE_OVERRIDE;
  static Uni<GLFWWindow> Create(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;

public:
  FIRE_CONSTEXPR const String& GetTitle() const FIRE_NOEXCEPT FIRE_OVERRIDE { return title; }
  FIRE_CONSTEXPR uint32_t GetWidth() const FIRE_NOEXCEPT FIRE_OVERRIDE { return width; }
  FIRE_CONSTEXPR uint32_t GetHeight() const FIRE_NOEXCEPT FIRE_OVERRIDE { return height; }
  FIRE_CONSTEXPR const void* GetNativeWindow() const FIRE_NOEXCEPT FIRE_OVERRIDE { return window; }
  void PollEvent() const FIRE_NOEXCEPT FIRE_OVERRIDE;
};

}

#endif //FIRE_GLFW_WINDOW_H
