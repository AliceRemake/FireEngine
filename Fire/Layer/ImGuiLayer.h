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
  VkDescriptorPool      descriptor_pool                                   = {};
  VkRenderPass          render_pass                                       = {};
  VkCommandPool         command_pool                                      = {};
  VkCommandBuffer       command_buffers[Application::MAX_FRAME_IN_FLIGHT] = {};
  Vector<VkFramebuffer> frame_buffers                                     = {};

public:
  explicit ImGuiLayer(LayerStack& layer_stack) FIRE_NOEXCEPT;
  ~ImGuiLayer() FIRE_NOEXCEPT FIRE_OVERRIDE = default;
  
  void       OnUpdate()                FIRE_NOEXCEPT FIRE_OVERRIDE;
  FireResult OnEvent(SDL_Event* event) FIRE_NOEXCEPT FIRE_OVERRIDE;
  void       OnResize()                FIRE_NOEXCEPT FIRE_OVERRIDE;
  void       OnAttach()                FIRE_NOEXCEPT FIRE_OVERRIDE;
  void       OnDetach()                FIRE_NOEXCEPT FIRE_OVERRIDE;

protected:
  virtual VkDescriptorPool CreateDescriptorPool(HRI::VulkanContext& VC) FIRE_NOEXCEPT;
  virtual VkRenderPass     CreateRenderPass(HRI::VulkanContext& VC)     FIRE_NOEXCEPT;
  virtual void             SetUpStyle()                                 FIRE_NOEXCEPT;
  virtual void             SetUpLayout()                                FIRE_NOEXCEPT;
};

}

#endif //FIRE_IMGUI_LAYER_H
