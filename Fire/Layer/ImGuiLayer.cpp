/**
  ******************************************************************************
  * @file           : ImGuiLayer.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-28
  ******************************************************************************
  */



#include "Layer/ImGuiLayer.h"
#include "Window/SDL3Window.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>

namespace FIRE {

ImGuiLayer::ImGuiLayer(LayerStack& layer_stack) FIRE_NOEXCEPT
#ifdef NDEBUG
  : Layer(layer_stack) {}
#else
  : Layer(layer_stack, "ImGui Layer") {}
#endif

vk::DescriptorPool ImGuiLayer::CreateDescriptorPool(HRI::VulkanContext& VC) FIRE_NOEXCEPT {
  FIRE_CONSTEXPR vk::DescriptorPoolSize descriptor_pool_sizes[] = {
    vk::DescriptorPoolSize(
      vk::DescriptorType::eCombinedImageSampler,
      1
    )
  };
  return VC.GetDevice().createDescriptorPool(
    vk::DescriptorPoolCreateInfo(
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
      1,
      std::size(descriptor_pool_sizes),
      descriptor_pool_sizes
    )
  );
}

void ImGuiLayer::OnUpdate() FIRE_NOEXCEPT {
  Application& APP = layer_stack.GetApplication();
  const HRI::VulkanContext& VC = APP.GetVulkanContext();
  
  ImGui_ImplSDL3_NewFrame();
  ImGui_ImplVulkan_NewFrame();
  ImGui::NewFrame();

  SetUpLayout();
  
  ImGui::Render();
  ImDrawData* draw_data = ImGui::GetDrawData();

  const vk::CommandBuffer& CB = command_buffers[APP.GetFrame()];
  const vk::Framebuffer& FB = frame_buffers[APP.GetImage()];
  
  CB.reset(vk::CommandBufferResetFlags(0));
  CB.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

  vk::ClearValue clear_value = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
  
  CB.beginRenderPass(
    vk::RenderPassBeginInfo(
      render_pass,
      FB,
      vk::Rect2D(vk::Offset2D{0,0}, VC.GetImageExtent()),
      vk::ArrayProxyNoTemporaries<const vk::ClearValue>{clear_value},
      nullptr
    ), vk::SubpassContents::eInline
  );
  
  ImGui_ImplVulkan_RenderDrawData(draw_data, CB);

  CB.endRenderPass();
  
  CB.end();

  auto wait_dst_stage_mask = vk::PipelineStageFlags(vk::PipelineStageFlagBits::eColorAttachmentOutput);
  
  const vk::SubmitInfo submit_infos[] = {
    vk::SubmitInfo(
      { APP.GetImageReady() },
      { wait_dst_stage_mask },
      { CB },
      { APP.GetImageFinish() }
    )
  };
  VC.GetGraphicsQueue().submit(submit_infos, { GetFence() });

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }
  
  APP.SetPresent(true);
}

FireResult ImGuiLayer::OnEvent(SDL_Event* event) FIRE_NOEXCEPT {
  ImGui_ImplSDL3_ProcessEvent(event);
  return FIRE_SUCCESS;
}

void ImGuiLayer::OnResize() FIRE_NOEXCEPT {
  const Application& APP = layer_stack.GetApplication();
  const HRI::VulkanContext& VC = APP.GetVulkanContext();

  for (uint32_t i = 0; i < frame_buffers.size(); ++i) {
    VC.GetDevice().destroyFramebuffer(frame_buffers[i]);
  }
  
  frame_buffers.resize(VC.GetImageViews().size());
  for (uint32_t i = 0; i < VC.GetImageViews().size(); ++i) {
    frame_buffers[i] = VC.GetDevice().createFramebuffer(
      vk::FramebufferCreateInfo(
        vk::FramebufferCreateFlags(0),
        render_pass,
        { VC.GetImageView(i) },
        VC.GetImageExtent().width,
        VC.GetImageExtent().height,
        { 1 }, nullptr
      )
    );
  }
  
  // command_pool = VC.GetDevice().createCommandPool(
  //   vk::CommandPoolCreateInfo(
  //     vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
  //     VC.GetGraphicsFamily(),
  //     nullptr
  //   )
  // );

  // VC.GetDevice().freeCommandBuffers(command_pool, command_buffers);
  // command_buffers = VC.GetDevice().allocateCommandBuffers(
  //   vk::CommandBufferAllocateInfo(
  //     command_pool,
  //     vk::CommandBufferLevel::ePrimary,
  //     APP.MAX_FRAME_IN_FLIGHT, nullptr
  //   )
  // );
}

vk::RenderPass ImGuiLayer::CreateRenderPass(HRI::VulkanContext& VC) FIRE_NOEXCEPT {
  const vk::AttachmentDescription attachments[] = {
    vk::AttachmentDescription(
      vk::AttachmentDescriptionFlags(0),
      VC.GetSurfaceFormat().format,
      vk::SampleCountFlagBits::e1,
      vk::AttachmentLoadOp::eClear,
      vk::AttachmentStoreOp::eStore,
      vk::AttachmentLoadOp::eDontCare,
      vk::AttachmentStoreOp::eDontCare,
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::ePresentSrcKHR
    )
  };
  const vk::AttachmentReference attachment_references[] = {
    vk::AttachmentReference(
      0,
      vk::ImageLayout::eColorAttachmentOptimal
    )
  };
  const vk::SubpassDescription subpasses[] = {
    vk::SubpassDescription(
      vk::SubpassDescriptionFlags(0),
      vk::PipelineBindPoint::eGraphics,
      0, nullptr,
      std::size(attachment_references), attachment_references
    )
  };
  const vk::SubpassDependency dependencies[] = {
    vk::SubpassDependency(
      VK_SUBPASS_EXTERNAL,
      0,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::AccessFlagBits::eColorAttachmentWrite,
      vk::AccessFlagBits::eColorAttachmentWrite
    )
  };
  return VC.GetDevice().createRenderPass(
    vk::RenderPassCreateInfo(
      static_cast<vk::RenderPassCreateFlags>(0),
      std::size(attachments),
      attachments,
      std::size(subpasses),
      subpasses,
      std::size(dependencies),
      dependencies
    )
  );
}

void ImGuiLayer::OnAttach() FIRE_NOEXCEPT {
  const Application& APP = layer_stack.GetApplication();
  HRI::VulkanContext& VC = APP.GetVulkanContext();
  
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  SetUpStyle();
  
  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  descriptor_pool = CreateDescriptorPool(VC);

  render_pass = CreateRenderPass(VC);

  frame_buffers.resize(VC.GetImageViews().size());
  for (uint32_t i = 0; i < VC.GetImageViews().size(); ++i) {
    frame_buffers[i] = VC.GetDevice().createFramebuffer(
      vk::FramebufferCreateInfo(
        vk::FramebufferCreateFlags(0),
        render_pass,
        vk::ArrayProxyNoTemporaries<const vk::ImageView>(VC.GetImageView(i)),
        VC.GetImageExtent().width,
        VC.GetImageExtent().height,
        { 1 }, nullptr
      )
    );
  }
  
  command_pool = VC.GetDevice().createCommandPool(
    vk::CommandPoolCreateInfo(
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      VC.GetGraphicsFamily(),
      nullptr
    )
  );

  command_buffers = VC.GetDevice().allocateCommandBuffers(
    vk::CommandBufferAllocateInfo(
      command_pool,
      vk::CommandBufferLevel::ePrimary,
      APP.MAX_FRAME_IN_FLIGHT, nullptr
    )
  );
  
  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForVulkan(
    APP.GetWindow().GetNative()
  );
  ImGui_ImplVulkan_InitInfo init_info = {};
  init_info.Instance = VC.GetInstance();
  init_info.PhysicalDevice = VC.GetPhysicalDevice();
  init_info.Device = VC.GetDevice();
  init_info.QueueFamily = VC.GetGraphicsFamily();
  init_info.Queue = VC.GetGraphicsQueue();
  init_info.PipelineCache = VK_NULL_HANDLE;
  init_info.DescriptorPool = descriptor_pool;
  init_info.RenderPass = render_pass;
  init_info.Subpass = 0;
  init_info.MinImageCount = VC.GetImageCount();
  init_info.ImageCount = VC.GetImageCount();
  init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  init_info.Allocator = nullptr;
  init_info.CheckVkResultFn = nullptr;
  ImGui_ImplVulkan_Init(&init_info);
}

void ImGuiLayer::OnDetach() FIRE_NOEXCEPT {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::SetUpStyle() FIRE_NOEXCEPT {
  
}
  
void ImGuiLayer::SetUpLayout() FIRE_NOEXCEPT {

}

}
