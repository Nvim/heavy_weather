#pragma once

#include "heavy_weather/rendering/Backend/GL/GLAPI.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <glad/glad.h>
#include <heavy_weather/rendering/Buffer.hpp>

namespace weather::graphics {

class GLBuffer : public Buffer {
public:
  explicit GLBuffer(BufferDescriptor &desc, void *data = nullptr);
  ~GLBuffer() override;
  u32 Handle() const { return handle_; }
  GLint Target() const { return target_; }
  static GLTarget GLTarget(GLint t);

  GLBuffer(const GLBuffer &) = default;
  GLBuffer(GLBuffer &&) = delete;
  GLBuffer &operator=(const GLBuffer &) = delete;
  GLBuffer &operator=(GLBuffer &&) = delete;

private:
  u32 handle_;
  GLint target_;
};

} // namespace weather::graphics
