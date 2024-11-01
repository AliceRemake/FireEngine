/**
  ******************************************************************************
  * @file           : Layer.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-28
  ******************************************************************************
  */



#ifndef FIRE_LAYER_H
#define FIRE_LAYER_H

#include "Common.h"
#include "Layer/LayerStack.h"

namespace FIRE {

class FIRE_API Layer {
protected:
  LayerStack& layer_stack;
  vk::Fence fences[Application::MAX_FRAME_IN_FLIGHT] = {};
  
public:
  explicit Layer(LayerStack& layer_stack) FIRE_NOEXCEPT;
  virtual ~Layer() FIRE_NOEXCEPT = default;

  FIRE_NODISCARD LayerStack& GetLayerStack() const FIRE_NOEXCEPT { return layer_stack; }
  FIRE_NODISCARD vk::Fence&  GetFence()            FIRE_NOEXCEPT { return fences[layer_stack.GetApplication().GetFrame()]; }
  
  virtual void       OnUpdate()                FIRE_NOEXCEPT;
  virtual FireResult OnEvent(SDL_Event* event) FIRE_NOEXCEPT;
  virtual void       OnResize()                FIRE_NOEXCEPT;
  virtual void       OnAttach()                FIRE_NOEXCEPT = 0;
  virtual void       OnDetach()                FIRE_NOEXCEPT = 0;
  
#ifndef NDEBUG
protected:
  const String name;
  explicit Layer(LayerStack& layer_stack, const String& name = "Layer") FIRE_NOEXCEPT;
public:
  FIRE_CONSTEXPR const String& GetName() const FIRE_NOEXCEPT;
#endif
};

}

#endif //FIRE_LAYER_H
