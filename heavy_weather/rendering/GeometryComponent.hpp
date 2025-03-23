#pragma once

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/Types.hpp"
namespace weather::graphics {

// Vertex Buffer + Index Buffer combo for rendering:
struct GeometryComponent {
  UniquePtr<Buffer> vbuffer;
  UniquePtr<Buffer> ibuffer;

  GeometryComponent(UniquePtr<Buffer> vbuffer, UniquePtr<Buffer> ibuffer)
      : vbuffer(std::move(vbuffer)), ibuffer(std::move(ibuffer)) {}
  GeometryComponent(GeometryComponent &&other) noexcept
      : vbuffer(std::move(other.vbuffer)), ibuffer(std::move(other.ibuffer)) {
    HW_CORE_INFO("GeometryComponent Move constructor");
    HW_ASSERT(this->ibuffer->GetSize() > 0);
    HW_ASSERT(this->ibuffer->Type() == BufferType::IndexBuffer);
    HW_ASSERT(this->vbuffer->GetSize() > 0);
    HW_ASSERT(this->vbuffer->Type() == BufferType::VertexBuffer);
  }

  GeometryComponent &operator=(GeometryComponent &&other) noexcept {
    if (this != &other) {
      // Take resources from 'other' and make them ours
      this->vbuffer = std::move(other).vbuffer;
      this->ibuffer = std::move(other).ibuffer;
    }
    return *this;
  }

  GeometryComponent(const GeometryComponent &) = delete;
  GeometryComponent &operator=(const GeometryComponent &) = delete;
};

} // namespace weather::graphics
