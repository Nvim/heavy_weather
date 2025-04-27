#include "GLAPI.hpp"

#include "GLShader.hpp"
#include "GLVertexBuffer.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/platform/Platform.hpp"
#include "heavy_weather/rendering/Backend/GL/GLBuffer.hpp"
#include "heavy_weather/rendering/Backend/GL/GLRenderTarget.hpp"
#include "heavy_weather/rendering/Backend/GL/GLShaderProgram.hpp"
#include "heavy_weather/rendering/Backend/GL/GLTexture.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Texture.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <glad/glad.h>

#include <memory>

namespace weather::graphics {

// TODO: Configurable Init
GLBackendAPI::GLBackendAPI(u16 w, u16 h, bool depth, bool debug)
    : config_{w, h, depth, debug, 0, 0} { //
  PlatformLoadBackend(Backend::OpenGL);

  i32 flags; // NOLINT
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
#ifdef GL_CONTEXT_FLAG_DEBUG_BIT
  if (debug && flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    HW_CORE_DEBUG("OpenGL debug context enabled")
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(LogDebugMessage, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
  }
#endif

  glGetIntegerv(GL_MAJOR_VERSION, &config_.maj);
  glGetIntegerv(GL_MINOR_VERSION, &config_.min);
  HW_CORE_INFO("OpenGL version: {}.{}", config_.maj, config_.min);

  glViewport(0, 0, w, h);
  if (depth) {
    glEnable(GL_DEPTH_TEST);
  }

  state_.targets[GLTarget::VAO] = 0;
  state_.targets[GLTarget::VBO] = 0;
  state_.targets[GLTarget::EBO] = 0;
  state_.targets[GLTarget::UBO] = 0;
}

void GLBackendAPI::Resize(std::pair<u16, u16> new_sz) {
  // HW_CORE_TRACE("Resizing opengl to {}x{}", new_sz.first, new_sz.second);
  glViewport(0, 0, new_sz.first, new_sz.second);
  config_.viewport_w = new_sz.first;
  config_.viewport_h = new_sz.second;
}

std::pair<u16, u16> GLBackendAPI::ViewPort() const {
  return {config_.viewport_w, config_.viewport_h};
}

UniquePtr<Buffer> GLBackendAPI::CreateVertexBuffer(BufferDescriptor desc,
                                                   void *data) {
  HW_ASSERT(desc.layout);
  u32 vao{};
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  HW_CORE_TRACE("Created VAO #{} for VertexBuffer creation", vao);

  UniquePtr<Buffer> buf =
      std::unique_ptr<Buffer>(new GLVertexBuffer(desc, data, vao));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  state_.targets[GLTarget::VAO] = vao;
  return buf;
}

UniquePtr<Buffer> GLBackendAPI::CreateBuffer(BufferDescriptor desc,
                                             void *data) {
  UniquePtr<Buffer> buf = nullptr;
  if (desc.type == BufferType::VertexBuffer) {
    buf = CreateVertexBuffer(desc, data);
  } else {
    buf = std::make_unique<GLBuffer>(desc, data);
  }

  return buf;
}

UniquePtr<Shader> GLBackendAPI::CreateShader(SharedPtr<ShaderSource> src,
                                             ShaderType type) {
  UniquePtr<Shader> ptr = nullptr;
  ptr = std::unique_ptr<Shader>(new GLShader(type, src));

  bool status = ptr->Compile();
  if (!status || ptr->Status() != ShaderCompileStatus::Success) {
    HW_CORE_ERROR("Shader creation failed!");
    return nullptr;
  }
  return ptr;
}

SharedPtr<ShaderProgram>
GLBackendAPI::CreatePipeline(PipelineDescriptor &desc) {
  SharedPtr<ShaderProgram> pipeline = nullptr;
  pipeline = std::shared_ptr<ShaderProgram>(new GLShaderProgram(desc));
  HW_ASSERT(pipeline != nullptr);

  ShaderCompileStatus status = pipeline->Init();
  if (status != ShaderCompileStatus::Success) {
    HW_CORE_ERROR("Couldn't init pipeline!");
    return nullptr;
  }

  return pipeline;
}

SharedPtr<RenderTarget> GLBackendAPI::CreateRenderTarget() {
  std::pair<u16, u16> s{config_.viewport_w, config_.viewport_h};
  auto r = std::make_shared<GLRenderTarget>(s);
  return r;
}

SharedPtr<Texture> GLBackendAPI::CreateTexture(SharedPtr<Image> img) {
  return std::shared_ptr<Texture>(new GLTexture(img));
};

// Use the vbo's vao to bind vbo
void GLBackendAPI::BindVBO(u32 vbo, u32 vao) {
  if (state_.targets[GLTarget::VAO] != vao) {
    glBindVertexArray(vao);
    state_.targets[GLTarget::VAO] = vao;
  }
  state_.targets[GLTarget::VBO] = vbo;
}

// Bind the ebo to the last used vao
void GLBackendAPI::BindEBO(u32 ebo) {
  glBindVertexArray(state_.targets[GLTarget::VAO]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  state_.targets[GLTarget::EBO] = ebo;
}

void GLBackendAPI::BindShaderResource(const Buffer &buf, i32 binding) {
  const auto &glbuf = dynamic_cast<const GLBuffer &>(buf);
  if (state_.targets[GLBuffer::GLTarget(glbuf.Target())] != glbuf.Handle()) {
    state_.targets[GLBuffer::GLTarget(glbuf.Target())] = glbuf.Handle();
    glBindBuffer(glbuf.Target(), glbuf.Handle());
  }
  glBindBufferBase(glbuf.Target(), binding, glbuf.Handle());
}

void GLBackendAPI::SetVertexBuffer(const Buffer &buf) {
  HW_ASSERT_MSG(buf.Type() == BufferType::VertexBuffer, "Bad buffer type");
  const auto &glbuf = dynamic_cast<const GLVertexBuffer &>(buf);
  BindVBO(glbuf.Hanlde(), glbuf.VAO());
}

void GLBackendAPI::SetIndexBuffer(const Buffer &buf) {
  HW_ASSERT_MSG(buf.Type() == BufferType::IndexBuffer, "Bad buffer type");
  const auto &glbuf = dynamic_cast<const GLBuffer &>(buf);
  BindEBO(glbuf.Handle());
}

void GLBackendAPI::WriteBufferData(const Buffer &buf, void *data, u64 offset,
                                   u64 data_sz) {
  HW_ASSERT_MSG(buf.Type() == BufferType::UniformBuffer,
                "This is probably not what you want");
  HW_ASSERT(buf.Size() != 0);
  const auto &glbuf = dynamic_cast<const GLBuffer &>(buf);
  if (glbuf.Size() < data_sz) {
    HW_ASSERT_MSG(false, "Invalid write: data size is bigger than buffer");
    return;
  }

  if (state_.targets[GLBuffer::GLTarget(glbuf.Target())] != glbuf.Handle()) {
    state_.targets[GLBuffer::GLTarget(glbuf.Target())] = glbuf.Handle();
    glBindBuffer(glbuf.Target(), glbuf.Handle());
  }
  glBufferSubData(glbuf.Target(), static_cast<GLintptr>(offset),
                  static_cast<GLintptr>(data_sz), data);
}

void GLBackendAPI::UsePipeline(ShaderProgram &pipeline) {
  auto &glpipeline = dynamic_cast<GLShaderProgram &>(pipeline);
  if (glpipeline.Status() != ShaderCompileStatus::Success) {
    HW_CORE_ERROR("Can't bind uninitialized pipeline");
    return;
  }
  if (state_.program == pipeline.Handle()) {
    return;
  }
  glUseProgram(glpipeline.Handle());
  state_.program = pipeline.Handle();
}

void GLBackendAPI::SetRenderTarget(RenderTarget &target) {
  // TODO: validation
  if (state_.fbo != target.Handle()) {
    glBindFramebuffer(GL_FRAMEBUFFER, target.Handle());
    state_.fbo = target.Handle();
  }
}
void GLBackendAPI::RestoreRenderTarget() {
  if (state_.fbo != 0) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    state_.fbo = 0;
  }
}

void GLBackendAPI::Clear(glm::vec4 col) const {
  glClearColor(col.r, col.g, col.b, col.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GLBackendAPI::ClearDepthBuffer() const { glClear(GL_DEPTH_BUFFER_BIT); }

void GLBackendAPI::Render() { glDrawArrays(GL_TRIANGLES, 0, 36); }

void GLBackendAPI::RenderIndexed(u64 count) {
  i32 i = count;
  glDrawElements(GL_TRIANGLES, i, GL_UNSIGNED_INT, nullptr);
}

GLBackendAPI::~GLBackendAPI() { //
  HW_CORE_INFO("Shutting down OpenGL backend");
}

} // namespace weather::graphics
