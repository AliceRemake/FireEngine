/**
  ******************************************************************************
  * @file           : VulkanRender.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_VULKAN_RENDER_H
#define FIRE_VULKAN_RENDER_H

#include "Render.h"

namespace FIRE {

class VulkanRender final : public Render{
public:
  static FIRE_CONSTEXPR bool ClassOf(const Render* ptr) FIRE_NOEXCEPT {
    return ptr->GetKind() == FIRE_RENDER_KIND_VULKAN;
  }

private:
  VulkanRender() FIRE_NOEXCEPT;
  ~VulkanRender() FIRE_NOEXCEPT FIRE_OVERRIDE = default;

public:
  static VulkanRender* Create() FIRE_NOEXCEPT;
  static void Destroy(VulkanRender* render) FIRE_NOEXCEPT;

public:
  void Init() FIRE_NOEXCEPT FIRE_OVERRIDE {}
  void DrawIndexed() FIRE_NOEXCEPT FIRE_OVERRIDE {}
};

}

#endif //FIRE_VULKAN_RENDER_H
