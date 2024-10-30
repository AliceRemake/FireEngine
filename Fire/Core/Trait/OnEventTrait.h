/**
  ******************************************************************************
  * @file           : OnEventTrait.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-30
  ******************************************************************************
  */



#ifndef FIRE_ON_EVENT_TRAIT_H
#define FIRE_ON_EVENT_TRAIT_H

#include "Common.h"
#include "Event/Event.h"

namespace FIRE {

class OnEventTrait {
public:
  virtual ~OnEventTrait() FIRE_NOEXCEPT = default;
  virtual FireResult OnEvent(const Ref<Event>& event) FIRE_NOEXCEPT = 0;
};
  
}

#endif //FIRE_ON_EVENT_TRAIT_H
