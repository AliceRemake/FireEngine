/**
  ******************************************************************************
  * @file           : Loader.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-1
  ******************************************************************************
  */



#ifndef FIRE_LOADER_H
#define FIRE_LOADER_H

#include <Common.h>

namespace FIRE {

namespace GLTF {

FireResult LoadBinary(const Path& uri, Model& model) FIRE_NOEXCEPT;

FireResult LoadASCII(const Path& uri, Model& model) FIRE_NOEXCEPT;

}

}

#endif //FIRE_LOADER_H
