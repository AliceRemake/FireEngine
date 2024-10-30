/**
  ******************************************************************************
  * @file           : Layer.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-28
  ******************************************************************************
  */



#include "Layer/Layer.h"

namespace FIRE {

FireResult Layer::OnEvent(const Ref<Event> &event) FIRE_NOEXCEPT {
  FIRE_UNUSE(event);
  FIRE_CRITICAL("Can Not Call OnEvent On Abstract Layer!");
  return FIRE_FAILURE;
}
  
void Layer::OnUpdate() FIRE_NOEXCEPT {
  FIRE_CRITICAL("Can Not Call OnUpdate On Abstract Layer!");
}

#ifndef NDEBUG 
  
Layer::Layer(const String& name) FIRE_NOEXCEPT : name(name) {}
  
FIRE_CONSTEXPR const String& Layer::GetName() const FIRE_NOEXCEPT { return name; }

#endif
  
}