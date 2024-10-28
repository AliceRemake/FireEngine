/**
  ******************************************************************************
  * @file           : Layer.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-28
  ******************************************************************************
  */



#ifndef FIRE_LAYER_H
#define FIRE_LAYER_H

#include "Core.h"
#include "Event/Event.h"

namespace FIRE {

// ReSharper disable once CppClassCanBeFinal
class FIRE_API Layer {
public:
  virtual ~Layer() FIRE_NOEXCEPT = default;
  virtual void OnAttach() FIRE_NOEXCEPT;
  virtual void OnDetach() FIRE_NOEXCEPT;
  virtual void OnUpdate() FIRE_NOEXCEPT;
  virtual void OnEvent(const Event& event) FIRE_NOEXCEPT;

#ifndef NDEBUG
public:
  const String name;
  explicit Layer(const String& name = "Layer") FIRE_NOEXCEPT : name(name) {} 
  FIRE_CONSTEXPR const String& GetName() const FIRE_NOEXCEPT { return name; }
#endif
};

}

#endif //FIRE_LAYER_H
