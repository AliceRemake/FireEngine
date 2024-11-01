/**
  ******************************************************************************
  * @file           : VulkanContext.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-29
  ******************************************************************************
  */



#ifndef FIRE_VULKAN_CONTEXT_H
#define FIRE_VULKAN_CONTEXT_H

#include "Common.h"
#include "Window/SDL3Window.h"

namespace FIRE {

namespace HRI {

class VulkanContext {
private:
  SDL3Window*                    window              = nullptr;
  vk::Instance                   instance            = VK_NULL_HANDLE;
  Uni<vk::DispatchLoaderDynamic> loader              = nullptr;
  vk::SurfaceKHR                 surface             = VK_NULL_HANDLE;
  vk::PhysicalDevice             physical_device     = VK_NULL_HANDLE;
  vk::Device                     device              = VK_NULL_HANDLE;
  Optional<uint32_t>             graphics_family     = std::nullopt;
  vk::Queue                      graphics_queue      = VK_NULL_HANDLE;
  Optional<uint32_t>             transfer_family     = std::nullopt;
  vk::Queue                      transfer_queue      = VK_NULL_HANDLE;
  vk::SwapchainKHR               swap_chain          = VK_NULL_HANDLE;
  uint32_t                       image_count         = 0;
  vk::Extent2D                   image_extent        = {};
  vk::SurfaceFormatKHR           surface_format      = {};
  vk::PresentModeKHR             present_mode        = {};
  Vector<vk::Image>              images              = {};
  Vector<vk::ImageView>          image_views         = {};
  #ifndef NDEBUG
  vk::DebugUtilsMessengerEXT     messenger = VK_NULL_HANDLE;
  #endif

public:
  FIRE_NODISCARD explicit VulkanContext(SDL3Window* window) FIRE_NOEXCEPT;
  ~VulkanContext() FIRE_NOEXCEPT;
  
public:
  void OnResize() FIRE_NOEXCEPT;

public:
  FIRE_NODISCARD const vk::Instance&            GetInstance      ()                     const FIRE_NOEXCEPT { return instance; }
  FIRE_NODISCARD const vk::PhysicalDevice&      GetPhysicalDevice()                     const FIRE_NOEXCEPT { return physical_device; }
  FIRE_NODISCARD const vk::Device&              GetDevice        ()                     const FIRE_NOEXCEPT { return device; }
  FIRE_NODISCARD uint32_t                       GetGraphicsFamily()                     const FIRE_NOEXCEPT { return graphics_family.value(); }
  FIRE_NODISCARD vk::Queue                      GetGraphicsQueue ()                     const FIRE_NOEXCEPT { return device.getQueue(GetGraphicsFamily(), 0); }
  FIRE_NODISCARD uint32_t                       GetTransferFamily()                     const FIRE_NOEXCEPT { return transfer_family.value(); }
  FIRE_NODISCARD vk::Queue                      GetTransferQueue ()                     const FIRE_NOEXCEPT { return device.getQueue(GetTransferFamily(), 0); }
  FIRE_NODISCARD const vk::SwapchainKHR&        GetSwapChain     ()                     const FIRE_NOEXCEPT { return swap_chain; }
  FIRE_NODISCARD uint32_t                       GetImageCount    ()                     const FIRE_NOEXCEPT { return image_count; }
  FIRE_NODISCARD const vk::Extent2D&            GetImageExtent   ()                     const FIRE_NOEXCEPT { return image_extent; }
  FIRE_NODISCARD const vk::SurfaceFormatKHR&    GetSurfaceFormat ()                     const FIRE_NOEXCEPT { return surface_format; }
  FIRE_NODISCARD const vk::PresentModeKHR&      GetPresentMode   ()                     const FIRE_NOEXCEPT { return present_mode; }
  FIRE_NODISCARD const Vector<vk::Image>&       GetImages        ()                     const FIRE_NOEXCEPT { return images; }
  FIRE_NODISCARD const vk::Image&               GetImages        (const uint32_t index) const FIRE_NOEXCEPT { return images[index]; }
  FIRE_NODISCARD const Vector<vk::ImageView>&   GetImageViews    ()                     const FIRE_NOEXCEPT { return image_views; }
  FIRE_NODISCARD const vk::ImageView&           GetImageView     (const uint32_t index) const FIRE_NOEXCEPT { return image_views[index]; }
  #ifndef NDEBUG
  FIRE_NODISCARD const vk::DebugUtilsMessengerEXT& GetMessenger() const FIRE_NOEXCEPT { return messenger; }
  #endif
};

}
  
}

#endif //FIRE_VULKAN_CONTEXT_H
