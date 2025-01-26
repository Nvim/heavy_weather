#include "GLPipeline.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/rendering/Backend/GL/GLShader.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

ShaderCompileStatus GLPipeline::Init() {
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
} // namespace weather::graphics
