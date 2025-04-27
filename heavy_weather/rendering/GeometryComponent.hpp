#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

class Buffer;

// Vertex Buffer + Index Buffer combo for rendering:
struct GeometryComponent {
  UniquePtr<Buffer> vbuffer;
  UniquePtr<Buffer> ibuffer;

  explicit GeometryComponent(UniquePtr<Buffer> vbuffer,
                             UniquePtr<Buffer> ibuffer);
  explicit GeometryComponent(GeometryComponent &&other) noexcept;
  GeometryComponent &operator=(GeometryComponent &&other) noexcept;

  GeometryComponent(const GeometryComponent &) = delete;
  GeometryComponent &operator=(const GeometryComponent &) = delete;
};

} // namespace weather::graphics
