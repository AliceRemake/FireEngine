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

#include "Core.h"
#include "Event/Event.h"

namespace FIRE {

class FIRE_API Window {
public:
  enum WindowKind : uint32_t{
    FIRE_WINDOW_KIND_SDL2,
    FIRE_WINDOW_KIND_GLFW,
  };

private:
  const WindowKind kind;

public:
  FIRE_CONSTEXPR WindowKind GetKind() const FIRE_NOEXCEPT { return kind; }

public:
  explicit Window(WindowKind kind) FIRE_NOEXCEPT;
  virtual ~Window() FIRE_NOEXCEPT = default;
  static Uni<Window> CreateSDL2(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  static Uni<Window> CreateGLFW(const char* title, uint32_t width, uint32_t height) FIRE_NOEXCEPT;
  
public:
  virtual FIRE_CONSTEXPR const String& GetTitle() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR uint32_t GetWidth() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR uint32_t GetHeight() const FIRE_NOEXCEPT = 0;
  virtual FIRE_CONSTEXPR const void* GetNativeWindow() const FIRE_NOEXCEPT = 0;
  virtual void PollEvent() const FIRE_NOEXCEPT = 0;

public:
  Function<void(Event&&)> event_callback = nullptr;
  void SetEventCallback(const Function<void(Event&&)>& callback) FIRE_NOEXCEPT;
};

}

#endif //FIRE_WINDOW_H
