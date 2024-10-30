/**
  ******************************************************************************
  * @file           : ImGuiLayer.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-28
  ******************************************************************************
  */



#ifndef FIRE_IMGUI_LAYER_H
#define FIRE_IMGUI_LAYER_H

#include "Layer/Layer.h"
#include "Core/Application.h"
#include "HRI/Vulkan/VulkanContext.h"
#include <imgui.h>

namespace FIRE {

// ReSharper disable once CppClassCanBeFinal
class FIRE_API ImGuiLayer : public Layer {
public:
  FireResult OnEvent(const Ref<Event>& event) FIRE_NOEXCEPT FIRE_OVERRIDE;
  void OnUpdate() FIRE_NOEXCEPT FIRE_OVERRIDE;
  void OnAttach() FIRE_NOEXCEPT FIRE_OVERRIDE;
  void OnDetach() FIRE_NOEXCEPT FIRE_OVERRIDE;
  
private:
  const Application& application;
  vk::DescriptorPool descriptor_pool;
  vk::RenderPass render_pass;
  vk::CommandPool command_pool;
  vk::CommandBuffer command_buffer;

protected:
  virtual vk::DescriptorPool CreateDescriptorPool(const HRI::VulkanContext& VC) FIRE_NOEXCEPT;
  virtual vk::RenderPass CreateRenderPass(const HRI::VulkanContext& VC) FIRE_NOEXCEPT;
  virtual void SetUpStyle() FIRE_NOEXCEPT;
  virtual void SetUpLayout() FIRE_NOEXCEPT;

public:
  explicit ImGuiLayer(const Application& application) FIRE_NOEXCEPT;
  ~ImGuiLayer() FIRE_NOEXCEPT FIRE_OVERRIDE = default;
};

}

#endif //FIRE_IMGUI_LAYER_H
