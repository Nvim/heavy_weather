#include "GLAPI.hpp"

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

UniquePtr<Buffer> GLBackendAPI::CreateBuffer(BufferDescriptor desc,
                                             void *data) {
  UniquePtr<Buffer> ptr = nullptr;
  if (desc.type == BufferType::IndexBuffer) {
    // ptr = std::unique_ptr<Buffer>(new GLIndexBuffer(desc, data));
    HW_CORE_ERROR("Couldn't create buffer: Index buffer not supported");
  } else if (desc.type == BufferType::VertexBuffer) {
    HW_ASSERT(desc.layout != nullptr);
    ptr = std::unique_ptr<Buffer>(new GLVertexBuffer(desc, data));
  } else {
    HW_CORE_ERROR("Couldn't create buffer: Unkown type");
  }

  return ptr;
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

void GLBackendAPI::BindBuffer(Buffer &buf) {
  if (buf.Type() == BufferType::VertexBuffer) {
    auto *glbuf = dynamic_cast<GLVertexBuffer *>(&buf);
    glBindVertexArray(glbuf->VAO());
    glBindBuffer(GL_ARRAY_BUFFER, glbuf->Hanlde());
  } else if (buf.Type() == BufferType::IndexBuffer) {
    HW_CORE_ERROR("Index Buffer not supported!");
  }
}

void GLBackendAPI::UsePipeline(Pipeline &pipeline) {
  auto *glpipeline = dynamic_cast<GLPipeline *>(&pipeline);
  if (glpipeline->Status() != ShaderCompileStatus::Success) {
    HW_CORE_ERROR("Can't bind uninitialized pipeline");
    return;
  }
  glUseProgram(glpipeline->Handle());
}

void GLBackendAPI::Clear() const {
  glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLBackendAPI::Render() { glDrawArrays(GL_TRIANGLES, 0, 3); }

GLBackendAPI::~GLBackendAPI() { //
  HW_CORE_INFO("Shutting down OpenGL backend");
}

} // namespace weather::graphics
