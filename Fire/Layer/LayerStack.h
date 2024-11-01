/**
  ******************************************************************************
  * @file           : LayerStack.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-29
  ******************************************************************************
  */



#ifndef FIRE_LAYER_STACK_H
#define FIRE_LAYER_STACK_H

#include "Common.h"
#include "Core/Application.h"

namespace FIRE {

class Layer;

class FIRE_API LayerStack {
private:
  Application&   application;
  uint32_t       top;
  Vector<Layer*> layer_stack;

public:
  explicit LayerStack(Application& application) FIRE_NOEXCEPT;
  ~LayerStack() FIRE_NOEXCEPT;
  
  FIRE_NODISCARD Application& GetApplication() const FIRE_NOEXCEPT { return application; }
  FIRE_NODISCARD uint32_t     Size()           const FIRE_NOEXCEPT { return top; }
  FIRE_NODISCARD Vector<VkFence> CollectFences() const FIRE_NOEXCEPT;
  
  void       Push(Layer* layer)              FIRE_NOEXCEPT;
  Layer*     Pop()                           FIRE_NOEXCEPT;
  void       OnUpdate()                const FIRE_NOEXCEPT;
  FireResult OnEvent(SDL_Event* event) const FIRE_NOEXCEPT;
  void       OnResize()                const FIRE_NOEXCEPT;
  
};

}

#endif //FIRE_LAYER_STACK_H
