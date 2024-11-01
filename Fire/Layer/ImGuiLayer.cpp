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

#ifdef NDEBUG
ImGuiLayer::ImGuiLayer(LayerStack& layer_stack) FIRE_NOEXCEPT
  : Layer(layer_stack) {}
#else
ImGuiLayer::ImGuiLayer(LayerStack& layer_stack) FIRE_NOEXCEPT
  : Layer(layer_stack, "ImGui Layer") {}
#endif

VkDescriptorPool ImGuiLayer::CreateDescriptorPool(HRI::VulkanContext& VC) FIRE_NOEXCEPT {
  FIRE_CONSTEXPR VkDescriptorPoolSize descriptor_pool_sizes[] = {
    VkDescriptorPoolSize{
      .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      .descriptorCount = 1,
    },
  };
  const VkDescriptorPoolCreateInfo descriptor_pool_create_info {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
    .maxSets = 1,
    .poolSizeCount = 1,
    .pPoolSizes = descriptor_pool_sizes,
  };
  VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
  const VkResult result = vkCreateDescriptorPool(VC.GetDevice(), &descriptor_pool_create_info, nullptr, &descriptor_pool);
  FIRE_CHECK_VK(result);
  return descriptor_pool;
}

VkRenderPass ImGuiLayer::CreateRenderPass(HRI::VulkanContext& VC) FIRE_NOEXCEPT {
  const VkAttachmentDescription attachments[] = {
    VkAttachmentDescription {
      .flags = 0,
      .format = VC.GetSurfaceFormat().format,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    }
  };
  const VkAttachmentReference attachment_references[] = {
    VkAttachmentReference{
      .attachment = 0,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    }
  };
  const VkSubpassDescription subpasses[] = {
    VkSubpassDescription{
      .flags = 0,
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .inputAttachmentCount = 0,
      .pInputAttachments = nullptr,
      .colorAttachmentCount = 1,
      .pColorAttachments = attachment_references,
      .pResolveAttachments = nullptr,
      .pDepthStencilAttachment = nullptr,
      .preserveAttachmentCount = 0,
      .pPreserveAttachments = nullptr,
    }
  };
  const VkSubpassDependency dependencies[] = {
    VkSubpassDependency{
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      .dependencyFlags = 0,
    }
  };
  const VkRenderPassCreateInfo render_pass_create_info {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .attachmentCount = 1,
    .pAttachments = attachments,
    .subpassCount = 1,
    .pSubpasses = subpasses,
    .dependencyCount = 1,
    .pDependencies = dependencies,
  };
  VkRenderPass render_pass = VK_NULL_HANDLE;
  {
    const VkResult result = vkCreateRenderPass(VC.GetDevice(), &render_pass_create_info, nullptr, &render_pass);
    FIRE_CHECK_VK(result);
  }
  return render_pass;
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
  
  const VkCommandBuffer& CB = command_buffers[APP.GetFrame()];
  const VkFramebuffer& FB = frame_buffers[APP.GetImage()];

  FIRE_CONSTEXPR VkCommandBufferBeginInfo begin_info {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = nullptr,
    .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    .pInheritanceInfo = nullptr,
  };

  VkResult result = VK_SUCCESS;
  
  result = vkResetCommandBuffer(CB, 0);
  FIRE_CHECK_VK(result);
  result = vkBeginCommandBuffer(CB, &begin_info);
  FIRE_CHECK_VK(result);

  // TODO
  // vk::ClearValue clear_value = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
  VkClearValue clear_value = VkClearValue{ VkClearColorValue{ 0.45f, 0.50f, 0.55f, 1.00f } };
  
  const VkRenderPassBeginInfo render_pass_begin_info {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .pNext = nullptr,
    .renderPass = render_pass,
    .framebuffer = FB,
    .renderArea = VkRect2D{ VkOffset2D{0,0}, VC.GetImageExtent() },
    .clearValueCount = 1,
    .pClearValues = &clear_value,
  };
  vkCmdBeginRenderPass(CB, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

  ImGui_ImplVulkan_RenderDrawData(draw_data, CB);

  vkCmdEndRenderPass(CB);
  vkEndCommandBuffer(CB);

  FIRE_CONSTEXPR VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  
  const VkSubmitInfo submit_infos[] = {
    VkSubmitInfo{
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .pNext = nullptr,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = &APP.GetImageReady(),
      .pWaitDstStageMask = &wait_dst_stage_mask,
      .commandBufferCount = 1,
      .pCommandBuffers = &CB,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores = &APP.GetImageFinish(),
    }
  };

  result = vkQueueSubmit(VC.GetGraphicsQueue(), 1, submit_infos, GetFence());
  FIRE_CHECK_VK(result);

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
    vkDestroyFramebuffer(VC.GetDevice(), frame_buffers[i], nullptr);
  }
  
  frame_buffers.resize(VC.GetImageViews().size());
  VkFramebufferCreateInfo frame_buffer_create_info {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .renderPass = render_pass,
    .attachmentCount = 1,
    .pAttachments = nullptr,
    .width = VC.GetImageExtent().width,
    .height =VC.GetImageExtent().height,
    .layers = 1,
  };
  for (uint32_t i = 0; i < VC.GetImageViews().size(); ++i) {
    frame_buffer_create_info.pAttachments = &VC.GetImageView(i);
    const VkResult result = vkCreateFramebuffer(VC.GetDevice(), &frame_buffer_create_info, nullptr, &frame_buffers[i]);
    FIRE_CHECK_VK(result);
  }
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
  VkFramebufferCreateInfo frame_buffer_create_info {
    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .pNext = nullptr,
    .flags = 0,
    .renderPass = render_pass,
    .attachmentCount = 1,
    .pAttachments = nullptr,
    .width = VC.GetImageExtent().width,
    .height =VC.GetImageExtent().height,
    .layers = 1,
  };
  for (uint32_t i = 0; i < VC.GetImageViews().size(); ++i) {
    frame_buffer_create_info.pAttachments = &VC.GetImageView(i);
    const VkResult result = vkCreateFramebuffer(VC.GetDevice(), &frame_buffer_create_info, nullptr, &frame_buffers[i]);
    FIRE_CHECK_VK(result);
  }

  const VkCommandPoolCreateInfo command_pool_create_info {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = nullptr,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = VC.GetGraphicsFamily(),
  };
  {
    const VkResult result = vkCreateCommandPool(VC.GetDevice(), &command_pool_create_info, nullptr, &command_pool);
    FIRE_CHECK_VK(result);
  }

  const VkCommandBufferAllocateInfo command_buffer_allocate_info {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = nullptr,
    .commandPool = command_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = Application::MAX_FRAME_IN_FLIGHT,
  };
  {
    const VkResult result = vkAllocateCommandBuffers(VC.GetDevice(), &command_buffer_allocate_info, command_buffers);
    FIRE_CHECK_VK(result);
  }
  
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
  const ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.Fonts->Clear();
  const ImFont* font = io.Fonts->AddFontDefault();
  IM_ASSERT(font != nullptr);
  io.Fonts->AddFontFromFileTTF((std::filesystem::path(FIRE_STR(ASSET_DIR)) / "Font" / "Cousine-Regular.ttf").string().c_str(), 18.0f);
  io.Fonts->AddFontFromFileTTF((std::filesystem::path(FIRE_STR(ASSET_DIR)) / "Font" / "DroidSans.ttf").string().c_str(), 18.0f);
  io.Fonts->AddFontFromFileTTF((std::filesystem::path(FIRE_STR(ASSET_DIR)) / "Font" / "Karla-Regular.ttf").string().c_str(), 18.0f);
  io.Fonts->AddFontFromFileTTF((std::filesystem::path(FIRE_STR(ASSET_DIR)) / "Font" / "ProggyClean.ttf").string().c_str(), 18.0f);
  io.Fonts->AddFontFromFileTTF((std::filesystem::path(FIRE_STR(ASSET_DIR)) / "Font" / "ProggyTiny.ttf").string().c_str(), 18.0f);
  io.Fonts->AddFontFromFileTTF((std::filesystem::path(FIRE_STR(ASSET_DIR)) / "Font" / "Roboto-Medium.ttf").string().c_str(), 18.0f);

  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 5.3f;
  style.FrameRounding = 2.3f;
  style.ScrollbarRounding = 0;

  style.Colors[ImGuiCol_Text]                 = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
  style.Colors[ImGuiCol_TextDisabled]         = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
  style.Colors[ImGuiCol_ChildBg]              = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_PopupBg]              = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
  style.Colors[ImGuiCol_Border]               = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
  style.Colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
  style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
  style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
  style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
  style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
  style.Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
  style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
  style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
  style.Colors[ImGuiCol_CheckMark]            = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
  style.Colors[ImGuiCol_SliderGrab]           = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
  style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
  style.Colors[ImGuiCol_Button]               = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
  style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
  style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
  style.Colors[ImGuiCol_Header]               = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
  style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
  style.Colors[ImGuiCol_Separator]            = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  style.Colors[ImGuiCol_SeparatorHovered]     = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
  style.Colors[ImGuiCol_SeparatorActive]      = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
  style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
  style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
  style.Colors[ImGuiCol_PlotLines]            = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
  style.Colors[ImGuiCol_ModalWindowDimBg]     = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}
  
void ImGuiLayer::SetUpLayout() FIRE_NOEXCEPT {
  ImGui::ShowDemoWindow();
}

}
