/**
  ******************************************************************************
  * @file           : VulkanContext.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-29
  ******************************************************************************
  */



#include "VulkanContext.h"

namespace FIRE {

namespace HRI {

#ifndef NDEBUG
static VkBool32 VKAPI_PTR debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void*                                       pUserData
);
#endif

VulkanContext::VulkanContext(SDL3Window* window) FIRE_NOEXCEPT : window(window) {
  FIRE_CONSTEXPR vk::ApplicationInfo application_info(
    "Fire Engin",
    vk::makeApiVersion(0, 0,0,0),
    "Fire Engin",
    vk::makeApiVersion(0,0,0,0),
    VK_API_VERSION_1_3,
    nullptr
  );

  FIRE_CONSTEXPR const char* instance_layers[] = {
    #ifndef NDEBUG
    "VK_LAYER_KHRONOS_validation"
    #endif
  };

  #ifdef NDEBUG
  uint32_t instance_extensions_count = 0;
  const char* const* instance_extensions =
    SDL_Vulkan_GetInstanceExtensions(&instance_extensions_count);
  #else
  uint32_t sdl3_instance_extensions_count = 0;
  const char* const* sdl3_instance_extensions =
    SDL_Vulkan_GetInstanceExtensions(&sdl3_instance_extensions_count);
  Vector<const char*> instance_extensions;
  instance_extensions.reserve(sdl3_instance_extensions_count + 1);
  instance_extensions.assign(sdl3_instance_extensions, sdl3_instance_extensions + sdl3_instance_extensions_count);
  instance_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  #endif
  
  FIRE_CONSTEXPR vk::DebugUtilsMessengerCreateInfoEXT messenger_create_info(
    vk::DebugUtilsMessengerCreateFlagsEXT(0),
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
    | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
    | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
    | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
    | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
    | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
    debugCallback,
    nullptr,
    nullptr
  );
  
  instance = vk::createInstance(
    vk::InstanceCreateInfo(
      vk::InstanceCreateFlags(0),
      &application_info,
      std::size(instance_layers),
      instance_layers,
      #ifdef NDEBUG
      instance_extensions_count,
      instance_extensions,
      nullptr
      #else
      instance_extensions.size(),
      instance_extensions.data(),
      &messenger_create_info
      #endif
    )
  );

  loader = CreateUni<vk::DispatchLoaderDynamic>(instance, vkGetInstanceProcAddr);
  
  #ifndef NDEBUG
  messenger = instance.createDebugUtilsMessengerEXT(messenger_create_info, nullptr, *loader);
  #endif

  SDL_Vulkan_CreateSurface(
    window->GetNative(),
    instance,
    nullptr,
    reinterpret_cast<VkSurfaceKHR*>(&surface)
  );

  const Vector<vk::PhysicalDevice> physical_devices = instance.enumeratePhysicalDevices();

  for (uint32_t i = 0; i < physical_devices.size(); ++i) {
    if (physical_devices[i].getProperties().deviceType != vk::PhysicalDeviceType::eDiscreteGpu) {
      continue;
    }
    if (physical_devices[i].getProperties().apiVersion < VK_API_VERSION_1_3) {
      continue;
    }

    const Vector<vk::QueueFamilyProperties> properties = physical_devices[i].getQueueFamilyProperties();
    for (uint32_t j = 0; j < properties.size(); ++j) {
      if (properties[j].queueCount <= 0) {
        continue;
      }
      vk::Bool32 support = vk::False;
      if (physical_devices[i].getSurfaceSupportKHR(j, surface, &support) != vk::Result::eSuccess) {
        FIRE_WARN("Can Not Get Physical Device Surface Support Info");
        continue;
      }
      if (support != vk::True) {
        continue;
      }
      if (!(properties[j].queueFlags & vk::QueueFlagBits::eGraphics)) {
        continue;
      }
      if (!(properties[j].queueFlags & vk::QueueFlagBits::eTransfer)) {
        continue;
      }
      graphics_family = transfer_family = j;
      physical_device = physical_devices[i];
      break;
    }
  }

  if (!graphics_family.has_value() || !transfer_family.has_value()) {
    FIRE_CRITICAL("Can Not Find Suitable Physical Device");
    FIRE_EXIT_FAILURE();
  }

  const Vector<uint32_t> queue_families = {graphics_family.value()};

  Vector<vk::DeviceQueueCreateInfo> device_queue_create_infos(queue_families.size());

  const float priorities[] = { 0.0f };
  for (uint32_t i = 0; i < queue_families.size(); ++i) {
    device_queue_create_infos[i] = vk::DeviceQueueCreateInfo(
      vk::DeviceQueueCreateFlags(0),
      queue_families[i],
      1,
      priorities,
      nullptr
    );
  }

  const char* device_layers[] = {};
  const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  FIRE_UNUSE(device_layers);
  
  const vk::DeviceCreateInfo device_create_info(
    vk::DeviceCreateFlags(0),
    device_queue_create_infos.size(),
    device_queue_create_infos.data(),
    0,
    nullptr,
    std::size(device_extensions),
    device_extensions
  );
  
  device = physical_device.createDevice(device_create_info);

  vk::SurfaceCapabilitiesKHR capabilities = physical_device.getSurfaceCapabilitiesKHR(surface);
  
  image_count = std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);
  
  image_extent = capabilities.currentExtent;

  surface_format = physical_device.getSurfaceFormatsKHR(surface)[0];

  present_mode = physical_device.getSurfacePresentModesKHR(surface)[0];
  
  vk::SwapchainCreateInfoKHR swap_chain_create_info(
    vk::SwapchainCreateFlagsKHR(0),
    surface,
    image_count,
    surface_format.format,
    surface_format.colorSpace,
    image_extent,
    1,
    vk::ImageUsageFlagBits::eColorAttachment,
    vk::SharingMode::eExclusive,
    queue_families.size(),
    queue_families.data(),
    physical_device.getSurfaceCapabilitiesKHR(surface).currentTransform,
    vk::CompositeAlphaFlagBitsKHR::eOpaque,
    present_mode,
    vk::True,
    VK_NULL_HANDLE,
    nullptr
  );
  
  swap_chain = device.createSwapchainKHR(swap_chain_create_info);
  
  images = device.getSwapchainImagesKHR(swap_chain);

  image_views.resize(images.size());
  
  for (uint32_t i = 0; i < images.size(); ++i) {
    image_views[i] = device.createImageView(
      vk::ImageViewCreateInfo(
        vk::ImageViewCreateFlags(0),
        images[i],
        vk::ImageViewType::e2D,
        surface_format.format,
        vk::ComponentMapping(
          vk::ComponentSwizzle::eR,
          vk::ComponentSwizzle::eG,
          vk::ComponentSwizzle::eB,
          vk::ComponentSwizzle::eA
        ),
        vk::ImageSubresourceRange(
          vk::ImageAspectFlagBits::eColor,
          0,1,0,1
        ),
        nullptr
      )
    );
  }
}

VulkanContext::~VulkanContext() FIRE_NOEXCEPT {}

void VulkanContext::OnResize() FIRE_NOEXCEPT {
  device.waitIdle();
  
  const vk::SurfaceCapabilitiesKHR capabilities = physical_device.getSurfaceCapabilitiesKHR(surface);
  
  image_count = std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);
  
  image_extent = capabilities.currentExtent;

  const Vector<uint32_t> queue_families = {graphics_family.value()};
  
  const vk::SwapchainCreateInfoKHR swap_chain_create_info(
    vk::SwapchainCreateFlagsKHR(0),
    surface,
    image_count,
    surface_format.format,
    surface_format.colorSpace,
    image_extent,
    1,
    vk::ImageUsageFlagBits::eColorAttachment,
    vk::SharingMode::eExclusive,
    queue_families.size(),
    queue_families.data(),
    physical_device.getSurfaceCapabilitiesKHR(surface).currentTransform,
    vk::CompositeAlphaFlagBitsKHR::eOpaque,
    present_mode,
    vk::True,
    VK_NULL_HANDLE,
    nullptr
  );

  for (uint32_t i = 0; i < image_views.size(); ++i) {
    device.destroyImageView(image_views[i]);
  }
  
  device.destroySwapchainKHR(swap_chain);
  
  swap_chain = device.createSwapchainKHR(swap_chain_create_info);
  
  images = device.getSwapchainImagesKHR(swap_chain);

  image_views.resize(images.size());
  
  for (uint32_t i = 0; i < images.size(); ++i) {
    image_views[i] = device.createImageView(
      vk::ImageViewCreateInfo(
        vk::ImageViewCreateFlags(0),
        images[i],
        vk::ImageViewType::e2D,
        surface_format.format,
        vk::ComponentMapping(
          vk::ComponentSwizzle::eR,
          vk::ComponentSwizzle::eG,
          vk::ComponentSwizzle::eB,
          vk::ComponentSwizzle::eA
        ),
        vk::ImageSubresourceRange(
          vk::ImageAspectFlagBits::eColor,
          0,1,0,1
        ),
        nullptr
      )
    );
  }
}

#ifndef NDEBUG
static VkBool32 VKAPI_PTR debugCallback(
  // ReSharper disable once CppParameterMayBeConst
  VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
  // ReSharper disable once CppParameterMayBeConst
  VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  // ReSharper disable once CppParameterMayBeConstPtrOrRef
  void*                                       pUserData
){
  FIRE_UNUSE(pUserData);
  
  static const char* MSG_TYPE[] = {
    "GENERAL",
    "VALIDATION",
    "PERFORMANCE",
    "DEVICE_ADDRESS_BINDING",
  };

  if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    FIRE_TRACE("[{}]{}", MSG_TYPE[std::__countr_zero(messageTypes)], pCallbackData->pMessage);
  } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
    FIRE_INFO("[{}]{}", MSG_TYPE[std::__countr_zero(messageTypes)], pCallbackData->pMessage);
  } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    FIRE_WARN("[{}]{}", MSG_TYPE[std::__countr_zero(messageTypes)], pCallbackData->pMessage);
  } else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    FIRE_ERROR("[{}]{}", MSG_TYPE[std::__countr_zero(messageTypes)], pCallbackData->pMessage);
  } else {
    FIRE_CRITICAL("Unreachable");
    FIRE_EXIT_FAILURE();
  }

  return VK_FALSE;
}
#endif
  
}
  
}