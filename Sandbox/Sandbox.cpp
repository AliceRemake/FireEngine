/**
  ******************************************************************************
  * @file           : Sandbox.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include <Fire.h>

class Sandbox final : public FIRE::Application {
private:
  bool shut_down = false;
  
public:
  Sandbox() FIRE_NOEXCEPT : Application() {
    window = FIRE::Window::CreateGLFW("Fire Engine", 1280, 720);
    window->SetEventCallback(std::bind(Sandbox::OnEvent, this, std::placeholders::_1));
  }
  ~Sandbox() FIRE_NOEXCEPT FIRE_OVERRIDE = default;
  void Run() FIRE_NOEXCEPT FIRE_OVERRIDE {
    while(!shut_down) {
      window->PollEvent();
      OnUpdate();
    }
  }
  void OnEvent(const FIRE::Event& event) FIRE_NOEXCEPT FIRE_OVERRIDE {
    if (IsA<FIRE::WindowCloseEvent>(&event)) {
      shut_down = true;
    }
  }
  void OnUpdate() noexcept override {
    FIRE_INFO("Hello Fire Engine!");
  }
};

FIRE_NODISCARD FIRE::Application* CreateApplication() FIRE_NOEXCEPT
{
  return new Sandbox();
}
