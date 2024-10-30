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
#include "Layer.h"

namespace FIRE {

class FIRE_API LayerStack final : public OnEventTrait, OnUpdateTrait {
public:
  FireResult OnEvent(const Ref<Event>& event) FIRE_NOEXCEPT FIRE_OVERRIDE;

public:
  void OnUpdate() FIRE_NOEXCEPT FIRE_OVERRIDE;

private:
  uint32_t top;
  Vector<Layer*> layer_stack;

public:
  LayerStack() FIRE_NOEXCEPT;
  ~LayerStack() FIRE_NOEXCEPT FIRE_OVERRIDE;
  void Push(Layer* layer) FIRE_NOEXCEPT;
  Layer* Pop() FIRE_NOEXCEPT;

};

}

#endif //FIRE_LAYER_STACK_H
