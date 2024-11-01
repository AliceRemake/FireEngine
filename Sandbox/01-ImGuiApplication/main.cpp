/**
  ******************************************************************************
  * @file           : main.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-1
  ******************************************************************************
  */



#include <Fire.h>
#include <imgui.h>

class MyImGuiLayer final : public FIRE::ImGuiLayer {
public:
  explicit MyImGuiLayer(FIRE::LayerStack& layer_stack) FIRE_NOEXCEPT
    : ImGuiLayer(layer_stack) {}

  void SetUpStyle() FIRE_NOEXCEPT FIRE_OVERRIDE {
    
  }
  void SetUpLayout() FIRE_NOEXCEPT FIRE_OVERRIDE {
    ImGui::ShowDemoWindow();
  }
};

class ImGuiApplication final : public FIRE::Application {
public:
  ImGuiApplication() FIRE_NOEXCEPT : Application() {
    GetLayerStack().Push(new MyImGuiLayer(GetLayerStack()));
  }
};

FIRE_NODISCARD FIRE::Application* CreateApplication() FIRE_NOEXCEPT
{
  return new ImGuiApplication();
}
