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
  window         = CreateUni<SDL3Window>("Fire Engine", 1280, 720);
  vulkan_context = CreateUni<HRI::VulkanContext>(window.get());
  layer_stack    = CreateUni<LayerStack>(*this);
  
  for (uint32_t i = 0; i < MAX_FRAME_IN_FLIGHT; ++i) {
    image_ready[i] = vulkan_context->GetDevice().createSemaphore(vk::SemaphoreCreateInfo());
    image_finish[i] = vulkan_context->GetDevice().createSemaphore(vk::SemaphoreCreateInfo());
  }
}

void Application::OnUpdate() FIRE_NOEXCEPT {
  // FIXME: Test Best Way For Watting Fences
  const Vector<vk::Fence> fences = layer_stack->CollectFences();
  if (const vk::Result result = vulkan_context->GetDevice().waitForFences(
    fences, vk::True, UINT64_MAX
  ); result == vk::Result::eSuccess) {
      vulkan_context->GetDevice().resetFences(fences);
  } else if (result == vk::Result::eTimeout) {
    return;
  } else {
    FIRE_CRITICAL("Can Not Wait For Fence: {}", static_cast<size_t>(result));
    FIRE_EXIT_FAILURE();
  }

  if (NextImage() == FIRE_FAILURE) { return; }
  
  layer_stack->OnUpdate();

  if (!resized && present) {
    VkSemaphore wait_semaphore = image_finish[frame];
    VkSwapchainKHR swap_chain = vulkan_context->GetSwapChain();
    const VkPresentInfoKHR present_info = {
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .pNext = nullptr,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &wait_semaphore,
      .swapchainCount = 1,
      .pSwapchains = &swap_chain,
      .pImageIndices = &image,
      .pResults = nullptr,
    };
    if (const VkResult result = vkQueuePresentKHR(vulkan_context->GetGraphicsQueue(), &present_info);
      result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) {
    } else if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      resized = true;
    } else {
      FIRE_CRITICAL("Can Not Present Image: {}", static_cast<size_t>(result));
      FIRE_EXIT_FAILURE();
    }
    present = false;
  }
}

FireResult Application::OnEvent(SDL_Event* event) FIRE_NOEXCEPT {
  if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
    && event->window.windowID == SDL_GetWindowID(window->GetNative()))) {
    done = true;
    return FIRE_FAILURE;
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
  if (const vk::ResultValue<uint32_t> result = vulkan_context->GetDevice().acquireNextImageKHR(
    vulkan_context->GetSwapChain(),
    UINT64_MAX,
    { GetImageReady() },
    {}
  ); result.result == vk::Result::eSuccess || result.result == vk::Result::eSuboptimalKHR) {
    image = result.value;
    return FIRE_SUCCESS;
  } else if (result.result == vk::Result::eTimeout || result.result == vk::Result::eNotReady) {
    return FIRE_FAILURE;
  } else if (result.result == vk::Result::eErrorOutOfDateKHR) {
    resized = true;
    return FIRE_FAILURE;
  } else {
    FIRE_CRITICAL("Can Not Acquire Next Image: {}", static_cast<size_t>(result.result));
    FIRE_EXIT_FAILURE();
  }
}

void Application::Run() FIRE_NOEXCEPT {
  while(!done) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      OnEvent(&event);
    }
    OnUpdate();
    if (resized) {
      OnResize();
    }
    NextFrame();
  }
}

}
