/**
  ******************************************************************************
  * @file           : Entry.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_ENTRY_H
#define FIRE_ENTRY_H

#ifdef FIRE_PLATFORM_WINDOWS

#if defined(FIRE_USE_VULKAN_RENDER) && defined(FIRE_USE_VOLK_LOADER)
#include <volk.h>
#endif

extern FIRE::Application *CreateApplication() FIRE_NOEXCEPT;

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int main(int argc, char** argv) {
  FIRE::Logger::Init();

#if defined(FIRE_USE_VULKAN_RENDER) && defined(FIRE_USE_VOLK_LOADER)
  volkInitialize();
#endif
  
  FIRE::Application* app = CreateApplication();
  app->Run();
  delete app;

#if defined(FIRE_USE_VULKAN_RENDER) && defined(FIRE_USE_VOLK_LOADER)
  volkFinalize();
#endif
  
  return 0;
}

#endif

#endif //FIRE_ENTRY_H
