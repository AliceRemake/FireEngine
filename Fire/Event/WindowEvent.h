/**
  ******************************************************************************
  * @file           : WindowEvent.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_WINDOWEVENT_H
#define FIRE_WINDOWEVENT_H

#include "Event/Event.h"

namespace FIRE {

class WindowCloseEvent final : public Event {
  FIRE_RTTI_DERIVE(Event, FIRE_EVENT_KIND_WINDOW_CLOSE)
  
public:
  WindowCloseEvent() FIRE_NOEXCEPT;
  ~WindowCloseEvent() FIRE_NOEXCEPT FIRE_OVERRIDE = default;
};

}

#endif //FIRE_WINDOWEVENT_H
