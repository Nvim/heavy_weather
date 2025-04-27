#pragma once

#include <heavy_weather/engine.h>

namespace weather::graphics {

class Texture;

class RenderTarget {
public:
  RenderTarget() = default;
  virtual ~RenderTarget() = default;
  virtual u32 Handle() const = 0;
  virtual u32 ColorHandle() const = 0;
  virtual u32 DepthHandle() const = 0;

private:
};

} // namespace weather::graphics
