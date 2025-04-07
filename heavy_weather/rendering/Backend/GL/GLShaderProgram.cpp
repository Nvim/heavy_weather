#include "GLShaderProgram.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/rendering/Backend/GL/GLShader.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

ShaderCompileStatus GLShaderProgram::Init() {
  HW_ASSERT(status_ == ShaderCompileStatus::NotCompiled);

  auto *vert = dynamic_cast<GLShader *>(vertex_.get());
  auto *frag = dynamic_cast<GLShader *>(fragment_.get());

  handle_ = glCreateProgram();

  glAttachShader(handle_, vert->Hanlde());
  glAttachShader(handle_, frag->Hanlde());
  LinkAndValidate();
  return status_;
}

void GLShaderProgram::Reload() {
  waiting_ = 2; // TODO: change this when adding more shader stages

  // TODO: queue these for multithreaded processing:
  vertex_->ReloadSrc();
  fragment_->ReloadSrc();
}

void GLShaderProgram::OnResourceReload(const ResourceReloadEvent<Shader> &evt) {
  if (evt.GetResource() != vertex_.get() &&
      evt.GetResource() != fragment_.get()) {
    return;
  }
  if (waiting_ != 0) {
    waiting_--;
  }
  if (waiting_ == 0) {
    LinkAndValidate();
  }
}

void GLShaderProgram::BindUniform(UniformDescriptor &desc) {
  int loc = glGetUniformLocation(handle_, desc.name);
  // clang-format off
  switch (desc.format) {
  case DataFormat::Int:
    BindUniform1i(loc, desc.data); break;
  case DataFormat::Float:
    BindUniform1f(loc, desc.data); break;
  case DataFormat::Float2:
    BindUniform2f(loc, desc.data); break;
  case DataFormat::Float3:
    BindUniform3f(loc, desc.data); break;
  case DataFormat::Float4:
    BindUniform4f(loc, desc.data); break;
  case DataFormat::Mat3:
    BindUniformMat3f(loc, desc.data); break;
  case DataFormat::Mat4:
    BindUniformMat4f(loc, desc.data); break;
  default:
    HW_CORE_ERROR("Couldn't bind uniform for data type");
  }
  // clang-format on
}

void GLShaderProgram::LinkAndValidate() {
  glLinkProgram(handle_);
  glValidateProgram(handle_);

  // TODO: glDeleteShader should be called on both shader objects as there's
  // no need to keep them once linked. But this would mean re-creating them
  // when reloading
  i32 success{};
  glGetProgramiv(handle_, GL_LINK_STATUS, &success);
  if (!success) {
    char info[512];                                   // NOLINT
    glGetProgramInfoLog(handle_, 512, nullptr, info); // NOLINT
    HW_CORE_ERROR("Couldn't link GL shaders: {}", info);
    status_ = ShaderCompileStatus::Failed;
  }
  status_ = ShaderCompileStatus::Success;
}

} // namespace weather::graphics
