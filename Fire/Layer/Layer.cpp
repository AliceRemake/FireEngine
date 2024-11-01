/**
  ******************************************************************************
  * @file           : Layer.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-28
  ******************************************************************************
  */



#include "Layer/Layer.h"

namespace FIRE {

Layer::Layer(LayerStack& layer_stack) FIRE_NOEXCEPT : layer_stack(layer_stack) {
  const Application& APP = layer_stack.GetApplication();
  const HRI::VulkanContext& VC = APP.GetVulkanContext();
  FIRE_CONSTEXPR VkFenceCreateInfo fence_create_info {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };
  for (uint32_t i = 0; i < APP.MAX_FRAME_IN_FLIGHT; ++i) {
    const VkResult result = vkCreateFence(VC.GetDevice(), &fence_create_info, nullptr, &fences[i]);
    FIRE_CHECK_VK(result);
  }
}

FireResult Layer::OnEvent(SDL_Event* event) FIRE_NOEXCEPT {
  FIRE_UNUSE(event);
  FIRE_CRITICAL("Can Not Call OnEvent On Abstract Layer!");
  FIRE_EXIT_FAILURE();
}
  
void Layer::OnUpdate() FIRE_NOEXCEPT {
  FIRE_CRITICAL("Can Not Call OnUpdate On Abstract Layer!");
  FIRE_EXIT_FAILURE();
}

void Layer::OnResize() FIRE_NOEXCEPT {
  FIRE_CRITICAL("Can Not Call OnResize On Abstract Layer!");
  FIRE_EXIT_FAILURE();
}

#ifndef NDEBUG
  
Layer::Layer(LayerStack& layer_stack, const String& name) FIRE_NOEXCEPT : layer_stack(layer_stack), name(name) {
  const Application& APP = layer_stack.GetApplication();
  const HRI::VulkanContext& VC = APP.GetVulkanContext();
  FIRE_CONSTEXPR VkFenceCreateInfo fence_create_info {
    .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };
  for (uint32_t i = 0; i < APP.MAX_FRAME_IN_FLIGHT; ++i) {
    const VkResult result = vkCreateFence(VC.GetDevice(), &fence_create_info, nullptr, &fences[i]);
    FIRE_CHECK_VK(result);
  }
}
  
FIRE_CONSTEXPR const String& Layer::GetName() const FIRE_NOEXCEPT { return name; }

#endif
  
}