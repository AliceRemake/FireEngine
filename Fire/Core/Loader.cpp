/**
  ******************************************************************************
  * @file           : Loader.cpp
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-11-1
  ******************************************************************************
  */



#include "Loader.h"

namespace FIRE {

namespace GLTF {

static tinygltf::TinyGLTF loader;

FireResult LoadBinary(const Path& uri, Model& model) FIRE_NOEXCEPT {
  String err, warn;
  const bool result = loader.LoadBinaryFromFile(&model, &err, &warn, uri.string().c_str());
  if (!err.empty()) {
    // FIRE_ERROR("{}", err);
    return FIRE_FAILURE;
  }
  if (!warn.empty()) {
    // FIRE_WARN("{}", warn);
    return FIRE_FAILURE;
  }
  if (result == false) {
    // FIRE_CRITICAL("Can Not Load GLTF Model: {}", uri);
    return FIRE_FAILURE;
  }
  return FIRE_SUCCESS;  
}

FireResult LoadASCII(const Path& uri, Model& model) FIRE_NOEXCEPT {
  String err, warn;
  const bool result = loader.LoadASCIIFromFile(&model, &err, &warn, uri.string().c_str());
  if (!err.empty()) {
    // FIRE_ERROR("{}", err);
    return FIRE_FAILURE;
  }
  if (!warn.empty()) {
    // FIRE_WARN("{}", warn);
    return FIRE_FAILURE;
  }
  if (result == false) {
    // FIRE_CRITICAL("Can Not Load GLTF Model: {}", uri);
    return FIRE_FAILURE;
  }
  return FIRE_SUCCESS;  
}

}

}