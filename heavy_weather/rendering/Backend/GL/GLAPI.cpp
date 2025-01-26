#include "GLAPI.hpp"

#include "GLIndexBuffer.hpp"
#include "GLShader.hpp"
#include "GLVertexBuffer.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Backend/GL/GLPipeline.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <glad/glad.h>
#include <memory>

namespace weather::graphics {

GLBackendAPI::GLBackendAPI() { //
  HW_CORE_INFO("Initializing OpenGL backend");
  glViewport(0, 0, 1280, 720);
}

void GLBackendAPI::Resize(std::pair<u16, u16> new_sz) {
  // HW_CORE_TRACE("Resizing opengl to {}x{}", new_sz.first, new_sz.second);
  glViewport(0, 0, new_sz.first, new_sz.second);
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

  state_.vao = vao;
  return buf;
}

UniquePtr<Buffer> GLBackendAPI::CreateIndexBuffer(BufferDescriptor desc,
                                                  void *data) {
  glBindVertexArray(state_.vao);
  HW_CORE_TRACE("Binding VAO #{} for IndexBuffer creation", state_.vao);
  UniquePtr<Buffer> buf =
      std::unique_ptr<Buffer>(new GLIndexBuffer(desc, data));
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return buf;
}

UniquePtr<Buffer> GLBackendAPI::CreateBuffer(BufferDescriptor desc,
                                             void *data) {
  UniquePtr<Buffer> buf = nullptr;
  if (desc.type == BufferType::IndexBuffer) {
    buf = CreateIndexBuffer(desc, data);
  } else if (desc.type == BufferType::VertexBuffer) {
    buf = CreateVertexBuffer(desc, data);
  } else {
    HW_CORE_ERROR("Couldn't create buffer: Unkown type");
  }

  return buf;
}

UniquePtr<Shader> GLBackendAPI::CreateShader(ShaderDescriptor desc) {
  UniquePtr<Shader> ptr = nullptr;
  ptr = std::unique_ptr<Shader>(new GLShader(desc.type, desc.path));

  bool status = ptr->Compile();
  if (!status || ptr->Status() != ShaderCompileStatus::Success) {
    HW_CORE_ERROR("Shader creation failed!");
    return nullptr;
  }
  return ptr;
}

UniquePtr<Pipeline> GLBackendAPI::CreatePipeline(PipelineDescriptor &desc) {
  UniquePtr<Pipeline> pipeline = nullptr;
  pipeline = std::unique_ptr<Pipeline>(new GLPipeline(desc));

  ShaderCompileStatus status = pipeline->Init();
  if (status != ShaderCompileStatus::Success) {
    HW_CORE_ERROR("Couldn't init pipeline!");
    return nullptr;
  }

  return pipeline;
}

// Use the vbo's vao to bind vbo
void GLBackendAPI::BindVBO(u32 vbo, u32 vao) {
  if (state_.vao != vao) {
    glBindVertexArray(vao);
    state_.vao = vao;
  }
  state_.vbo = vbo;
}

// Bind the ebo to the last used vao
void GLBackendAPI::BindEBO(u32 ebo) {
  glBindVertexArray(state_.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  state_.ebo = ebo;
}

void GLBackendAPI::BindBuffer(const Buffer &buf) {
  if (buf.Type() == BufferType::VertexBuffer) {
    const auto &glbuf = dynamic_cast<const GLVertexBuffer &>(buf);
    BindVBO(glbuf.Hanlde(), glbuf.VAO());
  } else if (buf.Type() == BufferType::IndexBuffer) {
    const auto &glbuf = dynamic_cast<const GLIndexBuffer &>(buf);
    BindEBO(glbuf.Hanlde());
  }
}

void GLBackendAPI::UsePipeline(Pipeline &pipeline) {
  auto &glpipeline = dynamic_cast<GLPipeline &>(pipeline);
  if (glpipeline.Status() != ShaderCompileStatus::Success) {
    HW_CORE_ERROR("Can't bind uninitialized pipeline");
    return;
  }
  glUseProgram(glpipeline.Handle());
}

void GLBackendAPI::Clear() const {
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLBackendAPI::Render() { glDrawArrays(GL_TRIANGLES, 0, 3); }

void GLBackendAPI::RenderIndexed(u64 count) {
  i32 i = count;
  glDrawElements(GL_TRIANGLES, i, GL_UNSIGNED_INT, nullptr);
}

GLBackendAPI::~GLBackendAPI() { //
  HW_CORE_INFO("Shutting down OpenGL backend");
}

} // namespace weather::graphics
