/**
  ******************************************************************************
  * @file           : Entry.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



// ReSharper disable CppNonInlineFunctionDefinitionInHeaderFile
#ifndef FIRE_ENTRY_H
#define FIRE_ENTRY_H

#ifdef FIRE_PLATFORM_WINDOWS

extern FIRE::Application *CreateApplication() FIRE_NOEXCEPT;

int main(const int argc, char** argv) {
  (void)argc;
  (void)argv;
  
  // FIRE::Logger::SetPattern("");

  #ifdef FIRE_USE_SDL3_WINDOW
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    CLIENT_CRITICAL("Can Not Initialize SDL3 Context!: {}", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  CLIENT_INFO("Initialize SDL3 Context Success.");
  #else
  CLIENT_CRITICAL("No Window Backend Specified!");
  exit(EXIT_FAILURE);
  #endif

  FIRE::Application* app = CreateApplication();
  app->Run();
  
  
  #ifdef FIRE_USE_SDL3_WINDOW
  SDL_Quit();
  #else
  CLIENT_CRITICAL("No Window Backend Specified!");
  exit(EXIT_FAILURE);
  #endif
  
  return 0;
}

#elif defined(FIRE_PLATFORM_LINUX)

extern FIRE::Application *CreateApplication() FIRE_NOEXCEPT;

int main(int argc, char** argv) {
  CLIENT_CRITICAL("Not Impletemented Yet!");
  return EXIT_FAILURE;
}

#endif

#endif //FIRE_ENTRY_H
