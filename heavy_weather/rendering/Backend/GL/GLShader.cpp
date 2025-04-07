#include "GLShader.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <heavy_weather/core/Asserts.hpp>
#include <utility>

static GLenum GetType(weather::graphics::ShaderType type) {
  switch (type) {
  case weather::graphics::ShaderType::FragmentShader:
    return GL_FRAGMENT_SHADER;
  case weather::graphics::ShaderType::VertexShader:
    return GL_VERTEX_SHADER;
  }
  HW_CORE_ERROR("Unkown shader type: {}", (u8)type);
  return 0;
}

namespace weather::graphics {

GLShader::~GLShader() {
  if (this->Status() != ShaderCompileStatus::NotCompiled) {
    glDeleteShader(handle_);
  }
}

GLShader::GLShader(ShaderType type, SharedPtr<ShaderSource> src)
    : Shader{type, std::move(src)} {}

bool GLShader::Compile() {
  HW_ASSERT(Status() == ShaderCompileStatus::NotCompiled);
  HW_ASSERT(!SourceStr().empty())

  const char *source_cstr = SourceStr().c_str();
  handle_ = glCreateShader(GetType(this->Type()));
  glShaderSource(handle_, 1, &source_cstr, nullptr);
  glCompileShader(handle_);

  if (CheckError()) {
    this->SetCompiled(ShaderCompileStatus::Success);
    return true;
  }
  this->SetCompiled(ShaderCompileStatus::Failed);
  return false;
}

void GLShader::OnResourceReload(const ResourceReloadEvent<ShaderSource> &evt) {
  if (evt.GetType() != ResourceType::SHADER_SOURCE ||
      this->Path() != evt.GetResource()->Path()) {
    return;
  }
  HW_CORE_DEBUG("Shader: Handling ResourceReloadEvent for path {}",
                Path().c_str());
  // this->SetCompiled(ShaderCompileStatus::NotCompiled);
  const char *source_cstr = SourceStr().c_str();
  glShaderSource(handle_, 1, &source_cstr, nullptr);
  glCompileShader(handle_);

  if (CheckError()) {
    this->SetCompiled(ShaderCompileStatus::Success);
  }
  this->SetCompiled(ShaderCompileStatus::Failed);
  EventDispatch(ResourceReloadEvent<Shader>{this});
}

bool GLShader::CheckError() const {
  i32 result{};
  i32 len = MAX_SHADER_LOG;
  glGetShaderiv(handle_, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    char message[MAX_SHADER_LOG];                    // NOLINT
    glGetShaderInfoLog(handle_, len, &len, message); // NOLINT
    HW_CORE_ERROR("Failed to compile shader: {}", message);
    return false;
  }
  return true;
}

} // namespace weather::graphics
