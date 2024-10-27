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
public:
  void Run() FIRE_NOEXCEPT FIRE_OVERRIDE {
    const FIRE::Event* window_close_event = new FIRE::WindowCloseEvent();
    const FIRE::Event* window_resize_event = new FIRE::WindowResizeEvent();
    const FIRE::Event* window_minimize_event = new FIRE::WindowMinimizeEvent();

    FIRE_ASSERT(DynCast<FIRE::WindowCloseEvent>(window_close_event));
    FIRE_ASSERT(DynCast<FIRE::WindowResizeEvent>(window_resize_event));
    FIRE_ASSERT(DynCast<FIRE::WindowMinimizeEvent>(window_minimize_event));

    FIRE_INFO("{}", window_close_event->GetName());
    FIRE_INFO("{}", window_resize_event->GetName());
    FIRE_INFO("{}", window_minimize_event->GetName());
    
    FIRE_INFO("{}", window_close_event->ToString());
    FIRE_INFO("{}", window_resize_event->ToString());
    FIRE_INFO("{}", window_minimize_event->ToString());
    
    // FIRE::CreateSDL2(SDL_INIT_VIDEO);
    //
    // FIRE::Window* window = FIRE::CreateSDL2Window("Fire Engine", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);
    //
    // FIRE_ASSERT(IsA<FIRE::SDL2Window>(window));
    // const FIRE::SDL2Window* p = Cast<FIRE::SDL2Window>(window);
    //
    // FIRE_INFO("title: {}", p->GetTitle());
    // FIRE_INFO("width: {}", p->GetWidth());
    // FIRE_INFO("height: {}", p->GetHeight());
    // FIRE_INFO("ptr: {}", p->GetNativeWindow());
    //
    // SDL_Delay(1000);
    //
    // FIRE::DestroySDL2Window(window);
    //
    // FIRE::DestroySDL2();
  }
};

FIRE_NODISCARD FIRE::Application* CreateApplication() FIRE_NOEXCEPT
{
  return new Sandbox();
}
