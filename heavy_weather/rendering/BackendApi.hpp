/*
 * Renderer uses the rendering API to render a triangle.
 * Later, it will be split in different classes and abstractions.
 * For now simplicity
 */
#pragma once

#include "Buffer.hpp"
#include "Types.hpp"
#include <glm/glm.hpp>
#include <heavy_weather/engine.h>
#include <utility>

namespace weather::graphics {
class BackendAPI {
public:
  explicit BackendAPI() = default;
  virtual ~BackendAPI() = default;

  /* Resource creation: */
  virtual UniquePtr<Buffer> CreateBuffer(BufferDescriptor desc, void *data) = 0;
  virtual UniquePtr<Shader> CreateShader(ShaderDescriptor desc) = 0;
  virtual SharedPtr<ShaderProgram> CreatePipeline(PipelineDescriptor &desc) = 0;

  /* Resouce binding: */
  virtual void BindBuffer(const Buffer &buf) = 0;
  virtual void UsePipeline(ShaderProgram &pipeline) = 0;
  // virtual void BindUniform(UniformDescriptor &desc) = 0;

  // Draw Call using previously bound resources:
  // TODO: Clear as a single method, buffer common interface abstraction
  virtual void Clear(glm::vec4 col) const = 0;
  virtual void ClearDepthBuffer() const = 0;
  virtual void Render() = 0;
  virtual void RenderIndexed(u64 count) = 0;

  /* Misc: */
  virtual void Resize(std::pair<u16, u16> new_sz) = 0;
  virtual std::pair<u16, u16> ViewPort() const = 0;

  //
  BackendAPI(const BackendAPI &) = default;
  BackendAPI(BackendAPI &&) = delete;
  BackendAPI &operator=(const BackendAPI &) = default;
  BackendAPI &operator=(BackendAPI &&) = delete;
};
} // namespace weather::graphics
