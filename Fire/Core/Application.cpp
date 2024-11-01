/**
  ******************************************************************************
  * @file           : Application.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#include "Application.h"
#include "RHI/Vulkan/VulkanContext.h"
#include "Layer/LayerStack.h"
#include <imgui.h>

namespace FIRE {

Application::Application() FIRE_NOEXCEPT {
  window         = CreateRef<SDL3Window>("Fire Engine", 1280, 720);
  vulkan_context = CreateUni<HRI::VulkanContext>(window);
  layer_stack    = CreateUni<LayerStack>(*this);
  
  FIRE_CONSTEXPR VkSemaphoreCreateInfo semaphore_create_info {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
  };
  for (uint32_t i = 0; i < MAX_FRAME_IN_FLIGHT; ++i) {
    vkCreateSemaphore(vulkan_context->GetDevice(), &semaphore_create_info, nullptr, &image_ready[i]);
    vkCreateSemaphore(vulkan_context->GetDevice(), &semaphore_create_info, nullptr, &image_finish[i]);
  }
}

void Application::OnUpdate() FIRE_NOEXCEPT {
  const Vector<VkFence> fences = layer_stack->CollectFences();
  VkResult result = VK_SUCCESS;
  result = vkWaitForFences(vulkan_context->GetDevice(), fences.size(), fences.data(), VK_TRUE, UINT64_MAX);
  if (result == VK_TIMEOUT) { return; }
  if (result == VK_SUCCESS) {
    vkResetFences(vulkan_context->GetDevice(), fences.size(), fences.data());
  } else {
    FIRE_CHECK_VK(result);
  }

  if (NextImage() == FIRE_FAILURE) { return; }
  
  layer_stack->OnUpdate();

  if (resized) { return; }
  if (!present) { return; }

  present = false;
  
  const VkPresentInfoKHR present_info = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = nullptr,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &image_finish[frame],
    .swapchainCount = 1,
    .pSwapchains = &vulkan_context->GetSwapChain(),
    .pImageIndices = &image,
    .pResults = nullptr,
  };
  result = vkQueuePresentKHR(vulkan_context->GetGraphicsQueue(), &present_info);
  if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) { return; }
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    resized = true;
    return;
  }
  FIRE_CRITICAL("Can Not Acquire Next Image: {}", static_cast<uint32_t>(result));
  FIRE_EXIT_FAILURE();
}

FireResult Application::OnEvent(SDL_Event* event) FIRE_NOEXCEPT {
  if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
    && event->window.windowID == SDL_GetWindowID(window->GetNative()))) {
    done = true;
    return FIRE_FAILURE;
  }
  if (event->type == SDL_EVENT_WINDOW_RESIZED) {
    resized = true;
    return FIRE_FAILURE;
  }
  if (event->type == SDL_EVENT_WINDOW_MINIMIZED) {
    minimize = true;
  }
  if (event->type == SDL_EVENT_WINDOW_RESTORED) {
    minimize = false;
  }
  return layer_stack->OnEvent(event);
}

void Application::OnResize() FIRE_NOEXCEPT {
  vulkan_context->OnResize();
  layer_stack->OnResize();
  image = 0;
  resized = false;
}

FireResult Application::NextImage() FIRE_NOEXCEPT {
  const VkResult result = vkAcquireNextImageKHR(vulkan_context->GetDevice(), vulkan_context->GetSwapChain(), UINT64_MAX, GetImageReady(), VK_NULL_HANDLE, &image);
  
  if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) {
    return FIRE_SUCCESS;
  } 
  if (result == VK_TIMEOUT || result == VK_NOT_READY) {
    return FIRE_FAILURE;
  }
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    resized = true;
    return FIRE_FAILURE;
  }
  FIRE_CRITICAL("Can Not Acquire Next Image: {}", static_cast<uint32_t>(result));
  FIRE_EXIT_FAILURE();
}

void Application::Run() FIRE_NOEXCEPT {
  while(!done) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      OnEvent(&event);
    }
    if (!resized && !minimize) {
      OnUpdate();
    }
    if (resized) {
      OnResize();
    }
    NextFrame();
  }
}

}
