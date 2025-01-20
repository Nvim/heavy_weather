#pragma once

#include <heavy_weather/rendering/Buffer.hpp>

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

class GLIndexBuffer : public Buffer {
public:
  GLIndexBuffer(BufferDescriptor desc, void *indices);
  ~GLIndexBuffer() override;
  u32 Hanlde() const;
  u32 VAO() const;

  //
  GLIndexBuffer(const GLIndexBuffer &) = default;
  GLIndexBuffer(GLIndexBuffer &&) = delete;
  GLIndexBuffer &operator=(const GLIndexBuffer &) = delete;
  GLIndexBuffer &operator=(GLIndexBuffer &&) = delete;

private:
  u32 handle_; // ID given by GL on creation
  u32 vao_;    // VAO associated with buffer
  VertexLayout layout_;
};

} // namespace weather::graphics
