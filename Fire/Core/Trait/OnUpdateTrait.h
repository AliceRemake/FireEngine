/**
  ******************************************************************************
  * @file           : OnUpdateTrait.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-30
  ******************************************************************************
  */



#ifndef FIRE_ON_UPDATE_TRAIT_H
#define FIRE_ON_UPDATE_TRAIT_H

#include "Common.h"

namespace FIRE {

class OnUpdateTrait {
public:
  virtual ~OnUpdateTrait() FIRE_NOEXCEPT = default;
  virtual void OnUpdate() FIRE_NOEXCEPT = 0;
};
  
}

#endif //FIRE_ON_UPDATE_TRAIT_H
