/**
  ******************************************************************************
  * @file           : Application.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_APPLICATION_H
#define FIRE_APPLICATION_H

#include "Common.h"
#include "Window/Window.h"
#include "Layer/LayerStack.h"
#include "HRI/Vulkan/VulkanContext.h"

namespace FIRE {

class FIRE_API Application : public OnEventTrait, OnUpdateTrait {
public:
  FireResult OnEvent(const Ref<Event>& event) FIRE_NOEXCEPT FIRE_OVERRIDE;

public:
  void OnUpdate() FIRE_NOEXCEPT FIRE_OVERRIDE;

protected:
  bool DONE = false;
  Uni<Window> window = nullptr;
  Uni<LayerStack> LS = nullptr;
  Uni<HRI::VulkanContext> VC = nullptr;
  
protected:
  Application() FIRE_NOEXCEPT;
  ~Application() FIRE_NOEXCEPT FIRE_OVERRIDE;

public:
  const Window& GetWindow() const FIRE_NOEXCEPT;
  const LayerStack& GetLS() const FIRE_NOEXCEPT;
  const HRI::VulkanContext& GetVC() const FIRE_NOEXCEPT;
  virtual void Run() FIRE_NOEXCEPT;

};

}

#endif //FIRE_APPLICATION_H
