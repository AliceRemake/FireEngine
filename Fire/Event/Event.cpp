/**
  ******************************************************************************
  * @file           : Event.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Event.h"

namespace FIRE {

Event::Event(const EventKind kind) FIRE_NOEXCEPT : kind(kind) {}

}