#pragma once

#include "Types.hpp"
#include <glm/vec4.hpp>
#include <heavy_weather/engine.h>

namespace weather {
class Image;
class ShaderSource;

namespace graphics {

class Texture;
class Buffer;
class RenderTarget;

class BackendAPI {
public:
  explicit BackendAPI() = default;
  virtual ~BackendAPI() = default;

  /* Resource creation: */
  virtual UniquePtr<Buffer> CreateBuffer(BufferDescriptor desc,
                                         void *data = nullptr) = 0;
  virtual UniquePtr<Shader> CreateShader(SharedPtr<ShaderSource> src,
                                         ShaderType type) = 0;
  virtual SharedPtr<Texture> CreateTexture(SharedPtr<Image> img) = 0;
  virtual SharedPtr<ShaderProgram> CreatePipeline(PipelineDescriptor &desc) = 0;
  virtual SharedPtr<RenderTarget> CreateRenderTarget() = 0;

  /* Resouce binding: */
  virtual void BindShaderResource(const Buffer &buf, i32 binding) = 0;
  virtual void SetVertexBuffer(const Buffer &buf) = 0;
  virtual void SetIndexBuffer(const Buffer &buf) = 0;
  virtual void WriteBufferData(const Buffer &buf, void *data, u64 offset,
                               u64 data_sz) = 0;
  virtual void UsePipeline(ShaderProgram &pipeline) = 0;
  virtual void SetRenderTarget(RenderTarget &target) = 0;
  virtual void RestoreRenderTarget() = 0;

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
} // namespace graphics
} // namespace weather
