/**
  ******************************************************************************
  * @file           : Event.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_EVENT_H
#define FIRE_EVENT_H

#include "Core.h"

namespace FIRE {

class FIRE_API Event {
public:
  enum EventKind : uint32_t {
    FIRE_EVENT_KIND_WINDOW_CLOSE,
    FIRE_EVENT_KIND_WINDOW_RESIZE,
    FIRE_EVENT_KIND_WINDOW_MINIMIZE,
    FIRE_EVENT_KIND_MAXIMUM,
  };

private:
  const EventKind kind;

public:
  FIRE_CONSTEXPR EventKind GetKind() const FIRE_NOEXCEPT { return kind; }
  
protected:
  explicit Event(EventKind kind) FIRE_NOEXCEPT;
  virtual ~Event() FIRE_NOEXCEPT = default;

public:
  virtual FIRE_CONSTEXPR const char* GetName() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR String ToString() const FIRE_NOEXCEPT = 0;
};

}

#endif //FIRE_EVENT_H
