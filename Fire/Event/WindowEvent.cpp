/**
  ******************************************************************************
  * @file           : WindowEvent.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "WindowEvent.h"

namespace FIRE {

WindowCloseEvent::WindowCloseEvent() FIRE_NOEXCEPT : Event(FIRE_EVENT_KIND_WINDOW_CLOSE) {}

WindowResizeEvent::WindowResizeEvent() FIRE_NOEXCEPT : Event(FIRE_EVENT_KIND_WINDOW_RESIZE) {}

WindowMinimizeEvent::WindowMinimizeEvent() FIRE_NOEXCEPT : Event(FIRE_EVENT_KIND_WINDOW_MINIMIZE) {}

}