/**
  ******************************************************************************
  * @file           : Application.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_APPLICATION_H
#define FIRE_APPLICATION_H

#include "Core.h"
#include "Event/Event.h"
#include "Window/Window.h"

namespace FIRE {

class FIRE_API Application {
protected:
  Uni<Window> window = nullptr;
  
public:
  virtual ~Application() FIRE_NOEXCEPT = default;
  const Window& GetWindow() const FIRE_NOEXCEPT;
  virtual void Run() FIRE_NOEXCEPT = 0;
  virtual void OnUpdate() FIRE_NOEXCEPT = 0;
  virtual void OnEvent(const Event& event) FIRE_NOEXCEPT = 0;
};

}

#endif //FIRE_APPLICATION_H
