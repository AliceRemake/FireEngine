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
#include "Window/SDL3Window.h"
#include "RHI/Vulkan/VulkanContext.h"

namespace FIRE {

class LayerStack;

class FIRE_API Application {
public:
  static FIRE_CONSTEXPR uint32_t MAX_FRAME_IN_FLIGHT = 2;
protected:
  bool                    done           = false;
  bool                    resized        = false;
  bool                    present        = false;
  uint32_t                frame          = 0;
  uint32_t                image          = 0;
  Uni<SDL3Window>         window         = nullptr;
  Uni<LayerStack>         layer_stack    = nullptr;
  Uni<HRI::VulkanContext> vulkan_context = nullptr;
  vk::Semaphore           image_ready[MAX_FRAME_IN_FLIGHT]  = {};
  vk::Semaphore           image_finish[MAX_FRAME_IN_FLIGHT] = {};
  
protected:
  Application() FIRE_NOEXCEPT;
  virtual ~Application() FIRE_NOEXCEPT = default;
  
public:
  FIRE_NODISCARD bool Done()const FIRE_NOEXCEPT { return done; }
  FIRE_NODISCARD bool Resized()  const FIRE_NOEXCEPT { return resized; }
  FIRE_NODISCARD bool Present()  const FIRE_NOEXCEPT { return present; }

  FIRE_NODISCARD FIRE_CONSTEXPR uint32_t GetFrame() const FIRE_NOEXCEPT { return frame; }
  FIRE_NODISCARD FIRE_CONSTEXPR uint32_t GetImage() const FIRE_NOEXCEPT { return image; }
  FIRE_NODISCARD SDL3Window&         GetWindow()        const FIRE_NOEXCEPT { return *window; }
  FIRE_NODISCARD LayerStack&         GetLayerStack()    const FIRE_NOEXCEPT { return *layer_stack; }
  FIRE_NODISCARD HRI::VulkanContext& GetVulkanContext() const FIRE_NOEXCEPT { return *vulkan_context; }
  FIRE_NODISCARD vk::Semaphore&      GetImageReady()          FIRE_NOEXCEPT { return image_ready[frame]; }
  FIRE_NODISCARD vk::Semaphore&      GetImageFinish()         FIRE_NOEXCEPT { return image_finish[frame]; }
  
  void       SetDone(const bool b)    FIRE_NOEXCEPT { done = b; }
  void       SetResized(const bool b) FIRE_NOEXCEPT { resized = b; }
  void       SetPresent(const bool b) FIRE_NOEXCEPT { present = b; }
  void       NextFrame()              FIRE_NOEXCEPT { frame = (frame + 1) % MAX_FRAME_IN_FLIGHT; }
  FireResult NextImage()              FIRE_NOEXCEPT;
  
  virtual void       OnUpdate()                FIRE_NOEXCEPT;
  virtual FireResult OnEvent(SDL_Event* event) FIRE_NOEXCEPT;
  virtual void       OnResize()                FIRE_NOEXCEPT;
  virtual void       Run()                     FIRE_NOEXCEPT;

};

}

#endif //FIRE_APPLICATION_H
