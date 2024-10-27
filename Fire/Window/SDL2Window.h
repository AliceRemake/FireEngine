/**
  ******************************************************************************
  * @file           : SDL2Window.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_SDL2WINDOW_H
#define FIRE_SDL2WINDOW_H

#include <SDL.h>
#include "Window.h"

namespace FIRE {

void CreateSDL2();
void DestroySDL2();

class SDL2Window final : public Window {
private:
  String title{};
  SDL_Window* window = nullptr;
  uint32_t width = 0;
  uint32_t height = 0;

public:
  SDL2Window(const char* title, )
  ~SDL2Window();
  String& GetTitle();
  uint32_t GetWidth();
  uint32_t GetHeight();
  void* GetNativeWindow();
  
};

}

#endif //FIRE_SDL2WINDOW_H
