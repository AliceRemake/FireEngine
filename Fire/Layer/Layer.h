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

#include "Common.h"
#include "Core/Trait/OnEventTrait.h"
#include "Core/Trait/OnUpdateTrait.h"

namespace FIRE {

class FIRE_API Layer : public OnEventTrait, OnUpdateTrait {
public:
  FireResult OnEvent(const Ref<Event> &event) FIRE_NOEXCEPT FIRE_OVERRIDE;

public:
  void OnUpdate() FIRE_NOEXCEPT FIRE_OVERRIDE;
  
public:
  ~Layer() FIRE_NOEXCEPT FIRE_OVERRIDE = default;
  virtual void OnAttach() FIRE_NOEXCEPT = 0;
  virtual void OnDetach() FIRE_NOEXCEPT = 0;

#ifndef NDEBUG
protected:
  const String name;
  explicit Layer(const String& name = "Layer") FIRE_NOEXCEPT;
public:
  FIRE_CONSTEXPR const String& GetName() const FIRE_NOEXCEPT;
#endif
};

}

#endif //FIRE_LAYER_H
