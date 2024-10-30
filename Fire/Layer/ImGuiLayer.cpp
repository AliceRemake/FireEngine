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
#include "Core/Application.h"
#include "HRI/Vulkan/VulkanContext.h"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>

namespace FIRE {

ImGuiLayer::ImGuiLayer(const Application& application) FIRE_NOEXCEPT
  : Layer("ImGui Layer"), application(application){}

vk::DescriptorPool ImGuiLayer::CreateDescriptorPool(const HRI::VulkanContext& VC) FIRE_NOEXCEPT {
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

vk::RenderPass ImGuiLayer::CreateRenderPass(const HRI::VulkanContext& VC) FIRE_NOEXCEPT {
  const vk::AttachmentDescription attachments[] = {
    vk::AttachmentDescription(
      vk::AttachmentDescriptionFlags(0),
      VC.GetSurfaceFormat().format,
      vk::SampleCountFlagBits::e1,
      vk::AttachmentLoadOp::eClear, // TODO
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

void ImGuiLayer::SetUpStyle() FIRE_NOEXCEPT {
  
}
  
void ImGuiLayer::SetUpLayout() FIRE_NOEXCEPT {
  ImGui::ShowDemoWindow();
}
  
void ImGuiLayer::OnAttach() FIRE_NOEXCEPT {
  const HRI::VulkanContext& VC = application.GetVC();
  
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

  descriptor_pool = CreateDescriptorPool(application.GetVC());
  render_pass = CreateRenderPass(application.GetVC());
  command_pool = VC.GetDevice().createCommandPool(
    vk::CommandPoolCreateInfo(
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      VC.GetGraphicsFamily(),
      nullptr
    )
  );
  command_buffer = VC.GetDevice().allocateCommandBuffers(
    vk::CommandBufferAllocateInfo(
      command_pool,
      vk::CommandBufferLevel::ePrimary,
      1, nullptr
    )
  )[0];
  
  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForVulkan(
    static_cast<SDL_Window*>(application.GetWindow().GetNative())
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
  init_info.Subpass = 0; // TODO imgui can be one of user render pass or use a separate render pass
  init_info.MinImageCount = 2;
  init_info.ImageCount = 2;
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

void ImGuiLayer::OnUpdate() FIRE_NOEXCEPT {
  ImGui_ImplSDL3_NewFrame();
  ImGui_ImplVulkan_NewFrame();
  ImGui::NewFrame();

  SetUpLayout();
  
  ImGui::Render();
  ImDrawData* draw_data = ImGui::GetDrawData();
  ImGui_ImplVulkan_RenderDrawData(draw_data, command_buffer);

  
}

FireResult ImGuiLayer::OnEvent(const Ref<Event>& event) FIRE_NOEXCEPT {
  FIRE_UNUSE(event);
  return FIRE_SUCCESS;
}
  
}
