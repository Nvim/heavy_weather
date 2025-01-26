#pragma once

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Buffer.hpp"

namespace weather::graphics {

class Mesh {
public:
  Mesh(UniquePtr<Buffer> vbuf, UniquePtr<Buffer> ibuf)
      : vbuffer_(std::move(vbuf)), ibuffer_(std::move(ibuf)){};

  const Buffer &VertexBuffer() const { return *vbuffer_; }
  const Buffer &IndexBuffer() const { return *ibuffer_; }

private:
  UniquePtr<Buffer> vbuffer_;
  UniquePtr<Buffer> ibuffer_;
};

} // namespace weather::graphics
