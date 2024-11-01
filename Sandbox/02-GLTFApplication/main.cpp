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

using namespace FIRE;

class GLTFApplication final : public Application {
public:
  GLTFApplication() FIRE_NOEXCEPT : Application() {
    GetLayerStack().Push(new ImGuiLayer(GetLayerStack()));
    Model model;
    GLTF::LoadBinary(Path(FIRE_STR(ASSET_DIR)) / "Model" / "GLTF" / "Binary" / "Box.glb", model);
  }

public:
  // void       OnUpdate()                FIRE_NOEXCEPT FIRE_OVERRIDE {}
  // FireResult OnEvent(SDL_Event* event) FIRE_NOEXCEPT FIRE_OVERRIDE { return FIRE_SUCCESS; }
  // void       OnResize()                FIRE_NOEXCEPT FIRE_OVERRIDE {}
  // void       Run()                     FIRE_NOEXCEPT FIRE_OVERRIDE {}
};

FIRE_NODISCARD Application* CreateApplication() FIRE_NOEXCEPT
{
  return new GLTFApplication();
}
