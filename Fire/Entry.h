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

extern FIRE::Application *CreateApplication() FIRE_NOEXCEPT;

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int main(int argc, char** argv) {
  FIRE::Logger::Init();
  
  FIRE::Application* app = CreateApplication();
  app->Run();
  delete app;
  return 0;
}

#endif

#endif //FIRE_ENTRY_H
