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

class Sandbox final : public FIRE::Application {};

FIRE_NODISCARD FIRE::Application* CreateApplication() FIRE_NOEXCEPT
{
  return new Sandbox();
}
