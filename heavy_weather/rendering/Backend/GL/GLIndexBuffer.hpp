#pragma once

#include <heavy_weather/rendering/Buffer.hpp>

namespace weather::graphics {

class GLIndexBuffer : public Buffer {
public:
  explicit GLIndexBuffer(BufferDescriptor desc, void *indices);
  ~GLIndexBuffer() override;
  u32 Hanlde() const { return handle_; }

  //
  GLIndexBuffer(const GLIndexBuffer &) = default;
  GLIndexBuffer(GLIndexBuffer &&) = delete;
  GLIndexBuffer &operator=(const GLIndexBuffer &) = delete;
  GLIndexBuffer &operator=(GLIndexBuffer &&) = delete;
  const VertexLayout *Layout() const override;
  void SetLayout(VertexLayout &&layout) override;

private:
  u32 handle_; // ID given by GL on creation
};

} // namespace weather::graphics
