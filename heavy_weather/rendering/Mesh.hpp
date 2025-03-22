#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/Transform.hpp"

namespace weather::graphics {

class Mesh {
public:
  Mesh(UniquePtr<Buffer> vbuf, UniquePtr<Buffer> ibuf)
      : vbuffer_(std::move(vbuf)), ibuffer_(std::move(ibuf)) {};

  const Buffer &VertexBuffer() const { return *vbuffer_; }
  const Buffer &IndexBuffer() const { return *ibuffer_; }
  void *Color() { return &material_.color; }
  MaterialComponent &Material() { return material_; }
  Transform *Transform() { return &transform_; }
  u64 GetID() const { return id_; }
  void SetID(u64 id) { id_ = id; }

private:
  UniquePtr<Buffer> vbuffer_;
  UniquePtr<Buffer> ibuffer_;
  struct MaterialComponent material_;
  struct Transform transform_;
  u64 id_{};
};

} // namespace weather::graphics
