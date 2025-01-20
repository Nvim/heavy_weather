#pragma once

#include <heavy_weather/rendering/Buffer.hpp>

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

class GLVertexBuffer : public Buffer {
public:
  GLVertexBuffer(BufferDescriptor desc, void *vertices);
  ~GLVertexBuffer() override;
  u32 Hanlde() const { return handle_; }
  u32 VAO() const { return vao_; }

  //
  GLVertexBuffer(const GLVertexBuffer &) = default;
  GLVertexBuffer(GLVertexBuffer &&) = delete;
  GLVertexBuffer &operator=(const GLVertexBuffer &) = delete;
  GLVertexBuffer &operator=(GLVertexBuffer &&) = delete;

private:
  u32 handle_; // ID given by GL on creation
  u32 vao_;    // VAO associated with buffer
  VertexLayout layout_;
};

} // namespace weather::graphics
