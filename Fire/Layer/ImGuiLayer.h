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

namespace FIRE {

// ReSharper disable once CppClassCanBeFinal
class FIRE_API ImGuiLayer : public Layer {
private:
  vk::DescriptorPool        descriptor_pool;
  vk::RenderPass            render_pass;
  vk::CommandPool           command_pool;
  Vector<vk::CommandBuffer> command_buffers;
  Vector<vk::Framebuffer>   frame_buffers;

public:
  explicit ImGuiLayer(LayerStack& layer_stack) FIRE_NOEXCEPT;
  ~ImGuiLayer() FIRE_NOEXCEPT FIRE_OVERRIDE = default;
  void       OnUpdate()                FIRE_NOEXCEPT FIRE_OVERRIDE;
  FireResult OnEvent(SDL_Event* event) FIRE_NOEXCEPT FIRE_OVERRIDE;
  void       OnResize()                FIRE_NOEXCEPT FIRE_OVERRIDE;
  void       OnAttach()                FIRE_NOEXCEPT FIRE_OVERRIDE;
  void       OnDetach()                FIRE_NOEXCEPT FIRE_OVERRIDE;

protected:
  virtual vk::DescriptorPool CreateDescriptorPool(HRI::VulkanContext& VC) FIRE_NOEXCEPT;
  virtual vk::RenderPass     CreateRenderPass(HRI::VulkanContext& VC)     FIRE_NOEXCEPT;
  // virtual vk::RenderPass     ReCreateRenderPass(HRI::VulkanContext& VC)   FIRE_NOEXCEPT;
  virtual void               SetUpStyle()                                 FIRE_NOEXCEPT;
  virtual void               SetUpLayout()                                FIRE_NOEXCEPT;
};

}

#endif //FIRE_IMGUI_LAYER_H
