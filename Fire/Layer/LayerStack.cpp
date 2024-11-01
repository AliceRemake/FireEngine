/**
  ******************************************************************************
  * @file           : LayerStack.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-29
  ******************************************************************************
  */



#include "Layer.h"
#include "Layer/LayerStack.h"

namespace FIRE {
  
LayerStack::LayerStack(Application& application) FIRE_NOEXCEPT : application(application), top(0) {}

LayerStack::~LayerStack() FIRE_NOEXCEPT {
  for (uint32_t i = 0; i < layer_stack.size(); ++i) {
    delete layer_stack[i];
  }
}

FIRE_NODISCARD Vector<vk::Fence> LayerStack::CollectFences() const FIRE_NOEXCEPT {
  Vector<vk::Fence> fences;
  fences.reserve(top);
  for (uint32_t i = 0; i < top; ++i) {
    fences.emplace_back(layer_stack[i]->GetFence());
  }
  return fences;
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

void LayerStack::OnUpdate() const FIRE_NOEXCEPT {
  for (uint32_t i = top; i > 0; --i) {
    layer_stack[i-1]->OnUpdate();
  }
}

FireResult LayerStack::OnEvent(SDL_Event* event) const FIRE_NOEXCEPT {
  for (uint32_t i = 0; i < top; ++i) {
    if (layer_stack[i]->OnEvent(event) == FIRE_FAILURE) {
      return FIRE_FAILURE;
    }
  }
  return FIRE_SUCCESS;
}

void LayerStack::OnResize() const FIRE_NOEXCEPT {
  for (uint32_t i = 0; i < top; ++i) {
    layer_stack[i]->OnResize();
  }
}

}