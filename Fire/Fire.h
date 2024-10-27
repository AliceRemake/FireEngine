/**
  ******************************************************************************
  * @file           : Fire.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_H
#define FIRE_H

#ifdef FIRE_PLATFORM_WINDOWS
  #ifdef FIRE_SHARED_LIBRARY
    #define FIRE_API __declspec(dllexport)
  #else
    #define FIRE_API __declspec(dllimport)
  #endif
#endif

FIRE_API void Print();

#endif //FIRE_H
