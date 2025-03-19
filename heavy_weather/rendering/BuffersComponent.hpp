#pragma once

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/Types.hpp"
namespace weather::graphics {

// Vertex Buffer + Index Buffer combo for rendering:
struct BuffersComponent {
  UniquePtr<Buffer> vbuffer;
  UniquePtr<Buffer> ibuffer;

  BuffersComponent(UniquePtr<Buffer> vbuffer, UniquePtr<Buffer> ibuffer)
      : vbuffer(std::move(vbuffer)), ibuffer(std::move(ibuffer)) {}
  BuffersComponent(BuffersComponent &&other) noexcept
      : vbuffer(std::move(other.vbuffer)), ibuffer(std::move(other.ibuffer)) {
    HW_CORE_INFO("BuffersComponent Move constructor");
    HW_ASSERT(this->ibuffer->GetSize() > 0);
    HW_ASSERT(this->ibuffer->Type() == BufferType::IndexBuffer);
    HW_ASSERT(this->vbuffer->GetSize() > 0);
    HW_ASSERT(this->vbuffer->Type() == BufferType::VertexBuffer);
  }

  BuffersComponent &operator=(BuffersComponent && other)  noexcept {
    if (this != &other) {
      // Take resources from 'other' and make them ours
      this->vbuffer = std::move(other).vbuffer;
      this->ibuffer = std::move(other).ibuffer;
    }
    return *this;
  }

  BuffersComponent(const BuffersComponent &) = delete;
  BuffersComponent &operator=(const BuffersComponent &) = delete;
};

} // namespace weather::graphics
