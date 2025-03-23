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
  glLinkProgram(handle_);
  glValidateProgram(handle_);

  i32 success{};
  glGetProgramiv(handle_, GL_LINK_STATUS, &success);
  if (!success) {
    char info[512];                                   // NOLINT
    glGetProgramInfoLog(handle_, 512, nullptr, info); // NOLINT
    HW_CORE_ERROR("Couldn't link GL shaders: {}", info);
    status_ = ShaderCompileStatus::Failed;
    return status_;
  }
  status_ = ShaderCompileStatus::Success;
  return status_;
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

} // namespace weather::graphics
