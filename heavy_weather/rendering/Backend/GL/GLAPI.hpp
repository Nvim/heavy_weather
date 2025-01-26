#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <heavy_weather/rendering/BackendApi.hpp>

#include <heavy_weather/engine.h>
#include <utility>

namespace weather::graphics {

// Stores currently bound resources:
struct GLState {
  u32 vao = 0;
  u32 vbo = 0;
  u32 ebo = 0;
};

class GLBackendAPI : public BackendAPI {
public:
  explicit GLBackendAPI();
  ~GLBackendAPI() override;

  UniquePtr<Buffer> CreateBuffer(BufferDescriptor desc, void *data) override;
  UniquePtr<Shader> CreateShader(ShaderDescriptor desc) override;
  UniquePtr<Pipeline> CreatePipeline(PipelineDescriptor &desc) override;

  void BindBuffer(const Buffer &buf) override;
  void UsePipeline(Pipeline &pipeline) override;
  void Render() override;
  void RenderIndexed(u64 count) override;
  void Clear() const override;
  void Resize(std::pair<u16, u16> new_sz) override;

  //
  GLBackendAPI(const GLBackendAPI &) = delete;
  GLBackendAPI(GLBackendAPI &&) = delete;
  GLBackendAPI &operator=(const GLBackendAPI &) = delete;
  GLBackendAPI &operator=(GLBackendAPI &&) = delete;

private:
  GLState state_;
  void RestoreState(GLState &s) {
    state_.ebo = s.ebo;
    state_.vbo = s.vbo;
    state_.vao = s.vao;
  };

  UniquePtr<Buffer> CreateVertexBuffer(BufferDescriptor desc, void *data);
  UniquePtr<Buffer> CreateIndexBuffer(BufferDescriptor desc, void *data);
  void BindVBO(u32 vbo, u32 vao);
  void BindEBO(u32 ebo);
};
} // namespace weather::graphics
