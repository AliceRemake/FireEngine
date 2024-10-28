/**
  ******************************************************************************
  * @file           : GLFWWindow.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Window/Backend/GLFWWindow.h"
#include "Event/WindowEvent.h"
#include "Logger.h"

namespace FIRE {

static uint32_t GLFW_WINDOW_COUNT = 0;

GLFWWindow::GLFWWindow(const char* title, const uint32_t width, const uint32_t height) FIRE_NOEXCEPT
: Window(FIRE_WINDOW_KIND_GLFW), title(title), width(width), height(height), window(nullptr) {
  FIRE_ASSERT(GLFW_WINDOW_COUNT < UINT32_MAX);
  
  if(++GLFW_WINDOW_COUNT == 1) {
    if (glfwInit() != GLFW_TRUE) {
      FIRE_CRITICAL("Can Not Create GLFW!\n");
      FIRE_ASSERT(0);
    }
  }

  // TODO handle graphics api
  // glfwWindowHint(GLFW_CLIENT_API, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  
  window = glfwCreateWindow(width, height, title, nullptr, nullptr);

  if (window == nullptr) {
    FIRE_ERROR("Can Not Create GLFW Window!\n");
    FIRE_ASSERT(0);
  }

  glfwSetWindowUserPointer(window, &event_callback);
  
  glfwSetWindowCloseCallback(this->window, [](GLFWwindow* window) -> void {
    const auto event_callback = *static_cast<Function<void(Event &&)> *>(glfwGetWindowUserPointer(window));
    event_callback(WindowCloseEvent());
  });

  // TODO
}

GLFWWindow::~GLFWWindow() FIRE_NOEXCEPT {
  FIRE_ASSERT(GLFW_WINDOW_COUNT > 0);

  glfwDestroyWindow(window);
  if (--GLFW_WINDOW_COUNT == 0) {
    glfwTerminate();
  }
}

void GLFWWindow::PollEvent() const FIRE_NOEXCEPT {
  glfwPollEvents();
}

Uni<GLFWWindow> GLFWWindow::Create(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT {
  return CreateUni<GLFWWindow>(title, width, height);
}

}