#pragma once

#include "heavy_weather/rendering/RenderTarget.hpp"
namespace weather::graphics {

class GLRenderTarget : public RenderTarget {
public:
  GLRenderTarget(std::pair<i32, i32> size);
  ~GLRenderTarget() override;
  u32 Handle() const override { return handle_; }
  u32 RGBHandle() const override { return color_; }
  u32 DepthHandle() const override { return depth_; }

private:
  std::pair<i32, i32> size_;
  u32 handle_;
  u32 color_;
  u32 depth_;
};

class GLMultiPassRenderTarget : public MultiPassRenderTarget {
public:
  GLMultiPassRenderTarget(std::pair<i32, i32> size);
  ~GLMultiPassRenderTarget() override;
  u32 Handle() const override { return handle_; }
  u32 RGBHandle() const override { return rgb_; }
  u32 PositionHandle() const override { return position_; }
  u32 NormalHandle() const override { return normal_; }
  u32 DepthHandle() const override { return depth_; }

private:
  std::pair<i32, i32> size_;
  u32 handle_;
  u32 rgb_;
  u32 position_;
  u32 normal_;
  u32 depth_;
};

} // namespace weather::graphics
