/**
  ******************************************************************************
  * @file           : main.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-1
  ******************************************************************************
  */



#include <Fire.h>
#include <imgui.h>

class MyImGuiLayer final : public FIRE::ImGuiLayer {
public:
  explicit MyImGuiLayer(FIRE::LayerStack& layer_stack) FIRE_NOEXCEPT
    : ImGuiLayer(layer_stack) {}

  void SetUpStyle() FIRE_NOEXCEPT FIRE_OVERRIDE {
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
  void SetUpLayout() FIRE_NOEXCEPT FIRE_OVERRIDE {
    ImGui::ShowDemoWindow();
  }
};

class ImGuiApplication final : public FIRE::Application {
public:
  ImGuiApplication() FIRE_NOEXCEPT : Application() {
    GetLayerStack().Push(new MyImGuiLayer(GetLayerStack()));
  }
};

FIRE_NODISCARD FIRE::Application* CreateApplication() FIRE_NOEXCEPT
{
  return new ImGuiApplication();
}
