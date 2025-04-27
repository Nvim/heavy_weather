#include "GeometryComponent.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/rendering/Buffer.hpp"

namespace weather::graphics {

GeometryComponent::GeometryComponent(UniquePtr<Buffer> vbuffer,
                                     UniquePtr<Buffer> ibuffer)
    : vbuffer(std::move(vbuffer)), ibuffer(std::move(ibuffer)) {}
GeometryComponent::GeometryComponent(GeometryComponent &&other) noexcept
    : vbuffer(std::move(other.vbuffer)), ibuffer(std::move(other.ibuffer)) {
  // HW_CORE_INFO("GeometryComponent Move constructor");
  HW_ASSERT(this->ibuffer->Size() > 0);
  HW_ASSERT(this->ibuffer->Type() == BufferType::IndexBuffer);
  HW_ASSERT(this->vbuffer->Size() > 0);
  HW_ASSERT(this->vbuffer->Type() == BufferType::VertexBuffer);
}

GeometryComponent &
GeometryComponent::operator=(GeometryComponent &&other) noexcept {
  if (this != &other) {
    // Take resources from 'other' and make them ours
    this->vbuffer = std::move(other).vbuffer;
    this->ibuffer = std::move(other).ibuffer;
  }
  return *this;
}
} // namespace weather::graphics
