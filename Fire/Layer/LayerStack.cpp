/**
  ******************************************************************************
  * @file           : LayerStack.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-29
  ******************************************************************************
  */



#include "Layer/LayerStack.h"

namespace FIRE {

FireResult LayerStack::OnEvent(const Ref<Event>& event) FIRE_NOEXCEPT {
  for (uint32_t i = 0; i < top; ++i) {
    if (layer_stack[i]->OnEvent(event)) {
      return FIRE_SUCCESS;
    }
  }
  return FIRE_FAILURE;
}

void LayerStack::OnUpdate() FIRE_NOEXCEPT {
  for (uint32_t i = top; i > 0; --i) {
    layer_stack[i-1]->OnUpdate();
  }
}
  
LayerStack::LayerStack() FIRE_NOEXCEPT : top(0) {}

LayerStack::~LayerStack() FIRE_NOEXCEPT {
  for (uint32_t i = 0; i < layer_stack.size(); ++i) {
    delete layer_stack[i];
  }
}
  
void LayerStack::Push(Layer* layer) FIRE_NOEXCEPT {
  if (top == layer_stack.size()) {
    layer_stack.push_back(layer);
    ++top;
  } else if (layer == layer_stack[top]) {
    ++top;
  } else {
    delete layer_stack[top];
    layer_stack[top++] = layer;
  }
    layer->OnAttach();
}

Layer* LayerStack::Pop() FIRE_NOEXCEPT {
  if (layer_stack.empty()) {
    return nullptr;
  }
  layer_stack.back()->OnDetach();
  return layer_stack[--top];
}
  
}