#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <heavy_weather/rendering/BackendApi.hpp>

#include <heavy_weather/engine.h>

namespace weather::graphics {
class GLBackendAPI : public BackendAPI {
public:
  explicit GLBackendAPI();
  ~GLBackendAPI() override;

  UniquePtr<Buffer> CreateBuffer(BufferDescriptor desc, void *data) override;
  UniquePtr<Shader> CreateShader(ShaderDescriptor desc) override;
  UniquePtr<Pipeline> CreatePipeline(PipelineDescriptor &desc) override;

  void BindBuffer(Buffer &buf) override;
  void UsePipeline(Pipeline &pipeline) override;
  void Render() override;
  void Clear() const override;

  //
  GLBackendAPI(const GLBackendAPI &) = delete;
  GLBackendAPI(GLBackendAPI &&) = delete;
  GLBackendAPI &operator=(const GLBackendAPI &) = delete;
  GLBackendAPI &operator=(GLBackendAPI &&) = delete;
};
} // namespace weather::graphics
