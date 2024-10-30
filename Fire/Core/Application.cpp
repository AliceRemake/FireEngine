/**
  ******************************************************************************
  * @file           : Application.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Application.h"
#include "Event/WindowEvent.h"
#include "Layer/ImGuiLayer.h"
#include "HRI/Vulkan/VulkanContext.h"

namespace FIRE {

FireResult Application::OnEvent(const Ref<Event>& event) FIRE_NOEXCEPT {
  if (IsA<WindowCloseEvent>(event.get())) {
    DONE = true;
    return FIRE_SUCCESS;
  }
  return LS->OnEvent(event);
};
  
void Application::OnUpdate() FIRE_NOEXCEPT {
  FIRE_INFO("Application OnUpdate Called");
};
  
Application::Application() FIRE_NOEXCEPT {
  window = Window::Create("Fire Engine", 1280, 720);
  LS = CreateUni<LayerStack>();
  VC = CreateUni<HRI::VulkanContext>(window.get());
  LS->Push(new ImGuiLayer(*this));
}

Application::~Application() FIRE_NOEXCEPT {
}
  
const Window& Application::GetWindow() const FIRE_NOEXCEPT {
  return *window;
}

const LayerStack& Application::GetLS() const FIRE_NOEXCEPT {
  return *LS;
}

const HRI::VulkanContext& Application::GetVC() const FIRE_NOEXCEPT {
  return *VC;
}

void Application::Run() FIRE_NOEXCEPT {
  while(!DONE) {
    OnEvent(window->PollEvent());
    OnUpdate();
  }
};
  
}
