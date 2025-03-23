#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <glm/glm.hpp>
#include <heavy_weather/rendering/BackendApi.hpp>

#include <heavy_weather/engine.h>
#include <utility>

namespace weather::graphics {

// Stores currently bound resources:
struct GLState {
  u32 vao = 0;
  u32 vbo = 0;
  u32 ebo = 0;
  u32 program = 0;
};

// Configuration/Features for API:
struct GLConfig {
  u16 viewport_w, viewport_h;
  bool depth_test;
  bool debug_mode;
  i32 maj, min; // opengl version
};

class GLBackendAPI : public BackendAPI {
public:
  explicit GLBackendAPI(u16 w, u16 h, bool depth, bool debug);
  ~GLBackendAPI() override;

  UniquePtr<Buffer> CreateBuffer(BufferDescriptor desc, void *data) override;
  UniquePtr<Shader> CreateShader(ShaderDescriptor desc) override;
  SharedPtr<ShaderProgram> CreatePipeline(PipelineDescriptor &desc) override;

  void BindBuffer(const Buffer &buf) override;
  void UsePipeline(ShaderProgram &pipeline) override;
  // void BindUniform(UniformDescriptor &desc) override;
  void Render() override;
  void RenderIndexed(u64 count) override;
  void Clear(glm::vec4 col) const override;
  void ClearDepthBuffer() const override;
  void Resize(std::pair<u16, u16> new_sz) override;
  std::pair<u16, u16> ViewPort() const override;

  //
  GLBackendAPI(const GLBackendAPI &) = delete;
  GLBackendAPI(GLBackendAPI &&) = delete;
  GLBackendAPI &operator=(const GLBackendAPI &) = delete;
  GLBackendAPI &operator=(GLBackendAPI &&) = delete;

private:
  GLState state_;
  GLConfig config_;
  void RestoreState(GLState &s) {
    state_.ebo = s.ebo;
    state_.vbo = s.vbo;
    state_.vao = s.vao;
    state_.program = s.program;
  };

  UniquePtr<Buffer> CreateVertexBuffer(BufferDescriptor desc, void *data);
  UniquePtr<Buffer> CreateIndexBuffer(BufferDescriptor desc, void *data);
  void BindVBO(u32 vbo, u32 vao);
  void BindEBO(u32 ebo);
};
} // namespace weather::graphics
