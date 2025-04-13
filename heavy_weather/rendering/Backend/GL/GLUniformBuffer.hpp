#pragma once

#include "heavy_weather/rendering/VertexLayout.hpp"
#include <heavy_weather/rendering/Buffer.hpp>

namespace weather::graphics {

class GLUniformBuffer : public Buffer {
public:
  explicit GLUniformBuffer(BufferDescriptor desc, void *data);
  ~GLUniformBuffer() override;
  u32 Hanlde() const { return handle_; }
  const VertexLayout *Layout() const override { return &layout_; };
  void SetLayout(VertexLayout &&layout) override;

  //
  GLUniformBuffer(const GLUniformBuffer &) = default;
  GLUniformBuffer(GLUniformBuffer &&) = delete;
  GLUniformBuffer &operator=(const GLUniformBuffer &) = delete;
  GLUniformBuffer &operator=(GLUniformBuffer &&) = delete;

private:
  static inline u32 binding_count = 0;
  u32 handle_; // ID given by GL on creation
  VertexLayout layout_;
};

} // namespace weather::graphics
