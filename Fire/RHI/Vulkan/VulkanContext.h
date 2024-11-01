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
  Ref<SDL3Window>     window          = {};
  VkInstance          instance        = {};
  VkSurfaceKHR        surface         = {};
  VkPhysicalDevice    physical_device = {};
  VkDevice            device          = {};
  uint32_t            graphics_family = {};
  VkQueue             graphics_queue  = {};
  uint32_t            transfer_family = {};
  VkQueue             transfer_queue  = {};
  VkSwapchainKHR      swap_chain      = {};
  uint32_t            image_count     = {};
  VkExtent2D          image_extent    = {};
  VkSurfaceFormatKHR  surface_format  = {};
  VkPresentModeKHR    present_mode    = {};
  Vector<VkImage>     images          = {};
  Vector<VkImageView> image_views     = {};
  #ifndef NDEBUG
  VkDebugUtilsMessengerEXT messenger = {};
  #endif

public:
  FIRE_NODISCARD explicit VulkanContext(const Ref<SDL3Window>& window) FIRE_NOEXCEPT;
  virtual ~VulkanContext() FIRE_NOEXCEPT;

  FIRE_NODISCARD const VkInstance&          GetInstance      ()                     const FIRE_NOEXCEPT { return instance; }
  FIRE_NODISCARD const VkPhysicalDevice&    GetPhysicalDevice()                     const FIRE_NOEXCEPT { return physical_device; }
  FIRE_NODISCARD const VkDevice&            GetDevice        ()                     const FIRE_NOEXCEPT { return device; }
  FIRE_NODISCARD const uint32_t&            GetGraphicsFamily()                     const FIRE_NOEXCEPT { return graphics_family; }
  FIRE_NODISCARD const VkQueue&             GetGraphicsQueue ()                     const FIRE_NOEXCEPT { return graphics_queue; }
  FIRE_NODISCARD const uint32_t&            GetTransferFamily()                     const FIRE_NOEXCEPT { return transfer_family; }
  FIRE_NODISCARD const VkQueue&             GetTransferQueue ()                     const FIRE_NOEXCEPT { return transfer_queue; }
  FIRE_NODISCARD const VkSwapchainKHR&      GetSwapChain     ()                     const FIRE_NOEXCEPT { return swap_chain; }
  FIRE_NODISCARD const uint32_t&            GetImageCount    ()                     const FIRE_NOEXCEPT { return image_count; }
  FIRE_NODISCARD const VkExtent2D&          GetImageExtent   ()                     const FIRE_NOEXCEPT { return image_extent; }
  FIRE_NODISCARD const VkSurfaceFormatKHR&  GetSurfaceFormat ()                     const FIRE_NOEXCEPT { return surface_format; }
  FIRE_NODISCARD const VkPresentModeKHR&    GetPresentMode   ()                     const FIRE_NOEXCEPT { return present_mode; }
  FIRE_NODISCARD const VkImage&             GetImage         (const uint32_t index) const FIRE_NOEXCEPT { return images[index]; }
  FIRE_NODISCARD const Vector<VkImage>&     GetImages        ()                     const FIRE_NOEXCEPT { return images; }
  FIRE_NODISCARD const VkImageView&         GetImageView     (const uint32_t index) const FIRE_NOEXCEPT { return image_views[index]; }
  FIRE_NODISCARD const Vector<VkImageView>& GetImageViews    ()                     const FIRE_NOEXCEPT { return image_views; }
  #ifndef NDEBUG
  FIRE_NODISCARD const VkDebugUtilsMessengerEXT& GetMessenger() const FIRE_NOEXCEPT { return messenger; }
  #endif
  
  virtual VkPresentModeKHR ChoosePresentMode(const Vector<VkPresentModeKHR>& desired_present_modes = {}) const FIRE_NOEXCEPT;
  virtual VkSurfaceFormatKHR ChooseSurfaceFormat(const Vector<VkSurfaceFormatKHR>& desired_surface_formats = {}) const FIRE_NOEXCEPT;
  virtual void OnResize() FIRE_NOEXCEPT;

  template<typename T>
  FIRE_NODISCARD static Vector<T> RemoveDuplicate(const Vector<T>& origin) FIRE_NOEXCEPT {
    Vector<T> result;
    result.reserve(origin.size());
    for (size_t i = 0; i < origin.size(); ++i) {
      bool duplicate = false;
      for (size_t j = 0; j < result.size(); ++j) {
        if (origin[i] == result[j]) {
          duplicate = true;
          break;
        }
      }
      if (duplicate) { continue; }
      result.emplace_back(origin[i]);
    }
    return result;
  }
};

}
  
}

#endif //FIRE_VULKAN_CONTEXT_H
