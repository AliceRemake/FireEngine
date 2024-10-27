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
    FIRE::CreateSDL2(SDL_INIT_VIDEO);

    FIRE::Window* window = FIRE::CreateSDL2Window("Fire Engine", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Delay(1000);
    
    FIRE::DestroySDL2Window(window);
    
    FIRE::DestroySDL2();
  }
};

FIRE_NODISCARD FIRE::Application* CreateApplication() FIRE_NOEXCEPT
{
  return new Sandbox();
}
