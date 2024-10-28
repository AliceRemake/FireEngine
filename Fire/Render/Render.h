/**
  ******************************************************************************
  * @file           : Render.h
  * @author         : AliceRemake
  * @brief          : None
  * @attention      : None
  * @date           : 24-10-27
  ******************************************************************************
  */



#ifndef FIRE_RENDER_H
#define FIRE_RENDER_H

namespace FIRE {

#include "Core.h"

class FIRE_API Render {
public:
  enum RenderKind : uint32_t {
    FIRE_RENDER_KIND_VULKAN,
    FIRE_RENDER_KIND_OPENGL,
  };

private:
  const RenderKind kind;

public:
  FIRE_CONSTEXPR RenderKind GetKind() const FIRE_NOEXCEPT { return kind; }
  
protected:
  explicit Render(RenderKind kind) FIRE_NOEXCEPT;
  virtual ~Render() FIRE_NOEXCEPT = default;

public:
  virtual void Init() FIRE_NOEXCEPT = 0;
  virtual void DrawIndexed() FIRE_NOEXCEPT = 0;
};

}

#endif //FIRE_RENDER_H
