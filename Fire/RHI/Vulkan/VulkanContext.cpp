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

VulkanContext::VulkanContext(const Ref<SDL3Window>& window) FIRE_NOEXCEPT : window(window) {
  FIRE_CONSTEXPR VkApplicationInfo application_info {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = nullptr,
    .pApplicationName = nullptr,
    .applicationVersion = 0,
    .pEngineName = nullptr,
    .engineVersion = 0,
    .apiVersion = VK_API_VERSION_1_3,
  };

  #ifdef NDEBUG
  FIRE_CONSTEXPR uint32_t instance_layers_count = 0;
  FIRE_CONSTEXPR const char* instance_layers[] = nullptr;
  #else
  FIRE_CONSTEXPR uint32_t instance_layers_count = 1;
  FIRE_CONSTEXPR const char* instance_layers[] = { "VK_LAYER_KHRONOS_validation" };
  #endif
  
  #ifdef NDEBUG
  uint32_t sdl3_instance_extensions_count = 0;
  char const* const* sdl3_instance_extensions = SDL_Vulkan_GetInstanceExtensions(&sdl3_instance_extensions_count);
  Vector<const char*> instance_extensions;
  instance_extensions.reserve(sdl3_instance_extensions_count);
  instance_extensions.assign(sdl3_instance_extensions, sdl3_instance_extensions + sdl3_instance_extensions_count);
  #else
  uint32_t sdl3_instance_extensions_count = 0;
  char const* const* sdl3_instance_extensions = SDL_Vulkan_GetInstanceExtensions(&sdl3_instance_extensions_count);
  Vector<const char*> instance_extensions;
  instance_extensions.reserve(sdl3_instance_extensions_count + 1);
  instance_extensions.assign(sdl3_instance_extensions, sdl3_instance_extensions + sdl3_instance_extensions_count);
  instance_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  #endif

  #ifndef NDEBUG
  FIRE_CONSTEXPR VkDebugUtilsMessengerCreateInfoEXT messenger_create_info{
    .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    .pNext = nullptr,
    .flags = 0,
    .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
    .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
    .pfnUserCallback = debugCallback,
    .pUserData = nullptr,
  };
  #endif

  const VkInstanceCreateInfo instance_create_info {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    #ifdef NDEBUG
    .pNext = nullptr,
    #else
    .pNext = &messenger_create_info,
    #endif
    .flags = 0,
    .pApplicationInfo = &application_info,
    .enabledLayerCount = instance_layers_count,
    .ppEnabledLayerNames = instance_layers,
    .enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size()),
    .ppEnabledExtensionNames = instance_extensions.data(),
  };

  {
    const VkResult result = vkCreateInstance(&instance_create_info, nullptr, &instance);
    FIRE_CHECK_VK(result);
  }
  
  #ifndef NDEBUG
  {
    const auto f = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    if (f == nullptr) { FIRE_EXIT_FAILURE(); }
    const VkResult result = f(instance, &messenger_create_info, nullptr, &messenger);
    FIRE_CHECK_VK(result);
  }
  #endif

  if (!SDL_Vulkan_CreateSurface(window->GetNative(), instance,nullptr, &surface)) {
    FIRE_CRITICAL("{}", SDL_GetError());
    FIRE_EXIT_FAILURE();
  }

  uint32_t physical_device_count = 0;
  {
    const VkResult result = vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);
    FIRE_CHECK_VK(result);
  }
  Vector<VkPhysicalDevice> physical_devices(physical_device_count);
  {
    const VkResult result = vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());
    FIRE_CHECK_VK(result);
  }

  for (uint32_t i = 0; i < physical_devices.size(); ++i) {
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physical_devices[i], &properties);
    if (properties.apiVersion < VK_API_VERSION_1_3) { continue; }
    if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { continue; }

    uint32_t queue_family_property_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_family_property_count, nullptr);
    Vector<VkQueueFamilyProperties> queue_family_properties(queue_family_property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_devices[i], &queue_family_property_count, queue_family_properties.data());

    bool found = false;
    for (uint32_t j = 0; j < queue_family_properties.size(); ++j) {
      if (queue_family_properties[j].queueCount <= 0) { continue; }

      VkBool32 support = VK_FALSE;
      vkGetPhysicalDeviceSurfaceSupportKHR(physical_devices[i], j, surface, &support);
      if (support == VK_FALSE) { continue; }
      
      if (!(queue_family_properties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)) { continue; }

      graphics_family = j;
      found = true;
      break;
    }
    if (!found) continue;

    found = false;
    for (uint32_t j = 0; j < queue_family_properties.size(); ++j) {
      if (queue_family_properties[j].queueCount <= 0) { continue; }
      
      if (!(queue_family_properties[j].queueFlags & VK_QUEUE_TRANSFER_BIT)) { continue; }

      transfer_family = j;
      found = true;
      break;
    }
    if (!found) continue;

    physical_device = physical_devices[i];
    break;
  }

  if (physical_device == VK_NULL_HANDLE) {
    FIRE_CRITICAL("Can Not Find Suitable Physical Device");
    FIRE_EXIT_FAILURE();
  }

  const Vector<uint32_t> queue_families = RemoveDuplicate(Vector{ graphics_family, transfer_family });

  Vector<VkDeviceQueueCreateInfo> device_queue_create_infos;
  device_queue_create_infos.reserve(queue_families.size());

  const float priorities[] = { 0.0f };
  VkDeviceQueueCreateInfo device_queue_create_info {
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .queueFamilyIndex = 0,
    .queueCount = 1,
    .pQueuePriorities = priorities,
  };
  for (uint32_t i = 0; i < queue_families.size(); ++i) {
    device_queue_create_info.queueFamilyIndex = queue_families[i];
    device_queue_create_infos.emplace_back(device_queue_create_info);
  }

  FIRE_CONSTEXPR uint32_t device_layer_count = 0;
  FIRE_CONSTEXPR char* device_layers[] = {};
  FIRE_CONSTEXPR uint32_t device_extension_count = 1;
  FIRE_CONSTEXPR char const* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  VkPhysicalDeviceFeatures features{};
  
  const VkDeviceCreateInfo device_create_info {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .queueCreateInfoCount = static_cast<uint32_t>(device_queue_create_infos.size()),
    .pQueueCreateInfos = device_queue_create_infos.data(),
    .enabledLayerCount = device_layer_count,
    .ppEnabledLayerNames = device_layers,
    .enabledExtensionCount = device_extension_count,
    .ppEnabledExtensionNames = device_extensions,
    .pEnabledFeatures = &features,
  };

  {
    const VkResult result = vkCreateDevice(physical_device, &device_create_info, nullptr, &device);
    FIRE_CHECK_VK(result);
  }

  vkGetDeviceQueue(device, graphics_family, 0, &graphics_queue);
  vkGetDeviceQueue(device, transfer_family, 0, &transfer_queue);
  
  VkSurfaceCapabilitiesKHR capabilities{};
  {
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);
    FIRE_CHECK_VK(result);
  }
  
  image_count = std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);
  
  image_extent = capabilities.currentExtent;

  surface_format = VulkanContext::ChooseSurfaceFormat();

  present_mode = VulkanContext::ChoosePresentMode();

  const VkSwapchainCreateInfoKHR swap_chain_create_info {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = nullptr,
    .flags = 0,
    .surface = surface,
    .minImageCount = image_count,
    .imageFormat = surface_format.format,
    .imageColorSpace = surface_format.colorSpace,
    .imageExtent = image_extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode =  queue_families.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
    .queueFamilyIndexCount = static_cast<uint32_t>(queue_families.size()),
    .pQueueFamilyIndices = queue_families.data(),
    .preTransform = capabilities.currentTransform,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = present_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = VK_NULL_HANDLE,
  };

  {
    const VkResult result = vkCreateSwapchainKHR(device, &swap_chain_create_info, nullptr, &swap_chain);
    FIRE_CHECK_VK(result);
  }
  
  vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr);
  images.resize(image_count);
  vkGetSwapchainImagesKHR(device, swap_chain, &image_count, images.data());
  
  image_views.resize(images.size());

  VkImageViewCreateInfo image_view_create_info {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .image = VK_NULL_HANDLE,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = surface_format.format,
    .components = {
      .r = VK_COMPONENT_SWIZZLE_R,
      .g = VK_COMPONENT_SWIZZLE_G,
      .b = VK_COMPONENT_SWIZZLE_B,
      .a = VK_COMPONENT_SWIZZLE_A,
    },
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1,
    },
  };
  
  for (uint32_t i = 0; i < images.size(); ++i) {
    image_view_create_info.image = images[i];
    {
      const VkResult result = vkCreateImageView(device, &image_view_create_info, nullptr, &image_views[i]);
      FIRE_CHECK_VK(result);
    }
  }
}

VulkanContext::~VulkanContext() FIRE_NOEXCEPT {
  for (uint32_t i = 0; i < image_views.size(); ++i) {
    vkDestroyImageView(device, image_views[i], nullptr);
  }
  vkDestroySwapchainKHR(device, swap_chain, nullptr);
  vkDestroyDevice(device, nullptr);
  {
    const auto f = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (f == nullptr) { FIRE_EXIT_FAILURE(); }
    f(instance, messenger, nullptr);
  }
  vkDestroyInstance(instance, nullptr);
}

VkPresentModeKHR VulkanContext::ChoosePresentMode(const Vector<VkPresentModeKHR>& desired_present_modes) const FIRE_NOEXCEPT {
  uint32_t present_mode_count = 0;
  {
    const VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);
    FIRE_CHECK_VK(result);
  }
  Vector<VkPresentModeKHR> present_modes(present_mode_count);
  {
    const VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, present_modes.data());
    FIRE_CHECK_VK(result);
  }
  
  for (size_t i = 0; i < desired_present_modes.size(); ++i) {
    for (size_t j = 0; j < present_modes.size(); ++j) {
      if (desired_present_modes[i] == present_modes[j]) {
        return desired_present_modes[i];
      }
    }
  }
  
  return present_modes[0];
}

VkSurfaceFormatKHR VulkanContext::ChooseSurfaceFormat(const Vector<VkSurfaceFormatKHR>& desired_surface_formats) const FIRE_NOEXCEPT {
  uint32_t surface_format_count = 0;
  {
    const VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, nullptr);
    FIRE_CHECK_VK(result);
  }
  Vector<VkSurfaceFormatKHR> surface_formats(surface_format_count);
  {
    const VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &surface_format_count, surface_formats.data());
    FIRE_CHECK_VK(result);
  }
  
  for (size_t i = 0; i < desired_surface_formats.size(); ++i) {
    for (size_t j = 0; j < surface_formats.size(); ++j) {
      if (desired_surface_formats[i].format == surface_formats[j].format
        && desired_surface_formats[i].colorSpace == surface_formats[j].colorSpace) {
        return desired_surface_formats[i];
      }
    }
  }
  
  return surface_formats[0];
}

void VulkanContext::OnResize() FIRE_NOEXCEPT {
  vkDeviceWaitIdle(device);

  for (uint32_t i = 0; i < image_views.size(); ++i) {
    vkDestroyImageView(device, image_views[i], nullptr);
  }

  const VkSwapchainKHR old_swap_chain = swap_chain;

  VkSurfaceCapabilitiesKHR capabilities{};
  {
    const VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities);
    FIRE_CHECK_VK(result);
  }
  
  image_count = std::min(capabilities.minImageCount + 1, capabilities.maxImageCount);
  
  image_extent = capabilities.currentExtent;

  surface_format = ChooseSurfaceFormat();

  present_mode = ChoosePresentMode();

  const Vector<uint32_t> queue_families = RemoveDuplicate(Vector{ graphics_family, transfer_family });

  const VkSwapchainCreateInfoKHR swap_chain_create_info {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = nullptr,
    .flags = 0,
    .surface = surface,
    .minImageCount = image_count,
    .imageFormat = surface_format.format,
    .imageColorSpace = surface_format.colorSpace,
    .imageExtent = image_extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode =  queue_families.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
    .queueFamilyIndexCount = static_cast<uint32_t>(queue_families.size()),
    .pQueueFamilyIndices = queue_families.data(),
    .preTransform = capabilities.currentTransform,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = present_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = old_swap_chain,
  };

  {
    const VkResult result = vkCreateSwapchainKHR(device, &swap_chain_create_info, nullptr, &swap_chain);
    FIRE_CHECK_VK(result);
  }
  
  vkDestroySwapchainKHR(device, old_swap_chain, nullptr);

  vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr);
  images.resize(image_count);
  vkGetSwapchainImagesKHR(device, swap_chain, &image_count, images.data());
  
  image_views.resize(images.size());

  VkImageViewCreateInfo image_view_create_info {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .image = VK_NULL_HANDLE,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = surface_format.format,
    .components = {
      .r = VK_COMPONENT_SWIZZLE_R,
      .g = VK_COMPONENT_SWIZZLE_G,
      .b = VK_COMPONENT_SWIZZLE_B,
      .a = VK_COMPONENT_SWIZZLE_A,
    },
    .subresourceRange = {
      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1,
    },
  };
  
  for (uint32_t i = 0; i < images.size(); ++i) {
    image_view_create_info.image = images[i];
    {
      const VkResult result = vkCreateImageView(device, &image_view_create_info, nullptr, &image_views[i]);
      FIRE_CHECK_VK(result);
    }
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