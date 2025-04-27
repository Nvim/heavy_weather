#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <heavy_weather/engine.h>
#include <heavy_weather/rendering/BackendApi.hpp>
#include <unordered_map>

namespace weather::graphics {

enum class GLTarget : u8 { VAO, VBO, EBO, UBO };
// Stores currently bound resources:
struct GLState {
  u32 program = 0;
  std::unordered_map<GLTarget, u32> targets;
  u32 fbo = 0;
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

  UniquePtr<Buffer> CreateBuffer(BufferDescriptor desc,
                                 void *data = nullptr) override;
  UniquePtr<Shader> CreateShader(SharedPtr<ShaderSource> src,
                                 ShaderType type) override;
  SharedPtr<Texture> CreateTexture(SharedPtr<Image> img) override;
  SharedPtr<ShaderProgram> CreatePipeline(PipelineDescriptor &desc) override;
  SharedPtr<RenderTarget> CreateRenderTarget() override;

  void WriteBufferData(const Buffer &buf, void *data, u64 offset,
                       u64 data_sz) override;
  void BindShaderResource(const Buffer &buf, i32 binding) override;
  void SetVertexBuffer(const Buffer &buf) override;
  void SetIndexBuffer(const Buffer &buf) override;
  void UsePipeline(ShaderProgram &pipeline) override;
  void SetRenderTarget(RenderTarget &target) override;
  void RestoreRenderTarget() override;

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
    state_.targets = s.targets;
    state_.program = s.program;
  };

  UniquePtr<Buffer> CreateVertexBuffer(BufferDescriptor desc, void *data);
  void BindVBO(u32 vbo, u32 vao);
  void BindEBO(u32 ebo);
};
} // namespace weather::graphics
