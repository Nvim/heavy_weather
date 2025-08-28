#pragma once

#include <heavy_weather/engine.h>

namespace weather::graphics {

class Texture;

class RenderTarget {
public:
  RenderTarget() = default;
  virtual ~RenderTarget() = default;
  virtual u32 Handle() const = 0;
  virtual u32 RGBHandle() const = 0;
  virtual u32 DepthHandle() const = 0;

private:
};

class MultiPassRenderTarget : public RenderTarget {
public:
  MultiPassRenderTarget() = default;
  ~MultiPassRenderTarget() override = default;
  u32 Handle() const override = 0;
  u32 RGBHandle() const override = 0;
  u32 DepthHandle() const override = 0;
  virtual u32 PositionHandle() const = 0;
  virtual u32 NormalHandle() const = 0;

private:
};

} // namespace weather::graphics
