/**
  ******************************************************************************
  * @file           : Window.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_WINDOW_H
#define FIRE_WINDOW_H

#include "Common.h"
#include "Event/Event.h"

namespace FIRE {

class FIRE_API Window {
  FIRE_RTTI_BASE(
    Window,
    FIRE_WINDOW_KIND_SDL3,
  )

public:
  virtual FIRE_CONSTEXPR const String& GetTitle() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR uint32_t GetWidth() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR uint32_t GetHeight() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR void* GetNative() const FIRE_NOEXCEPT = 0;
  virtual Ref<Event> PollEvent() const FIRE_NOEXCEPT = 0;
  static Uni<Window> Create(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;

};

}

#endif //FIRE_WINDOW_H
