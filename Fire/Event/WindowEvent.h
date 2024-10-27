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
public:
  static FIRE_CONSTEXPR bool ClassOf(const Event* ptr) FIRE_NOEXCEPT {
    return ptr->GetKind() == FIRE_EVENT_KIND_WINDOW_CLOSE;
  }

public:
  WindowCloseEvent() FIRE_NOEXCEPT;
  ~WindowCloseEvent() FIRE_NOEXCEPT FIRE_OVERRIDE = default;

public:
  FIRE_CONSTEXPR const char *GetName() const FIRE_NOEXCEPT FIRE_OVERRIDE { return __STR(FIRE_EVENT_KIND_WINDOW_CLOSE); }
  FIRE_CONSTEXPR String ToString() const FIRE_NOEXCEPT FIRE_OVERRIDE { return String(GetName()); }
};

class WindowResizeEvent final : public Event {
public:
  static FIRE_CONSTEXPR bool ClassOf(const Event* ptr) FIRE_NOEXCEPT {
    return ptr->GetKind() == FIRE_EVENT_KIND_WINDOW_RESIZE;
  }

public:
  WindowResizeEvent() FIRE_NOEXCEPT;
  ~WindowResizeEvent() FIRE_NOEXCEPT FIRE_OVERRIDE = default;

public:
  FIRE_CONSTEXPR const char *GetName() const FIRE_NOEXCEPT FIRE_OVERRIDE { return __STR(FIRE_EVENT_KIND_WINDOW_RESIZE); }
  FIRE_CONSTEXPR String ToString() const FIRE_NOEXCEPT FIRE_OVERRIDE { return String(GetName()); }
};

class WindowMinimizeEvent final : public Event {
public:
  static FIRE_CONSTEXPR bool ClassOf(const Event* ptr) FIRE_NOEXCEPT {
    return ptr->GetKind() == FIRE_EVENT_KIND_WINDOW_MINIMIZE;
  }

public:
  WindowMinimizeEvent() FIRE_NOEXCEPT;
  ~WindowMinimizeEvent() FIRE_NOEXCEPT FIRE_OVERRIDE = default;

public:
  FIRE_CONSTEXPR const char *GetName() const FIRE_NOEXCEPT FIRE_OVERRIDE { return __STR(FIRE_EVENT_KING_WINDOW_MINIMIZE); }
  FIRE_CONSTEXPR String ToString() const FIRE_NOEXCEPT FIRE_OVERRIDE { return String(GetName()); }
};

}

#endif //FIRE_WINDOWEVENT_H
