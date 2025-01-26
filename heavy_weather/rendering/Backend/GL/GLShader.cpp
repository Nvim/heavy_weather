#include "GLShader.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <fstream>
#include <heavy_weather/core/Asserts.hpp>
#include <sstream>

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

bool GLShader::Compile() {
  HW_ASSERT(this->Status() == ShaderCompileStatus::NotCompiled);
  HW_ASSERT(!this->Path().empty())
  std::ifstream shader_file;
  std::string shader_code;
  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    shader_file.open(this->Path());
    std::stringstream s;

    s << shader_file.rdbuf();
    shader_file.close();

    shader_code = s.str();
  } catch (std::ifstream::failure &e) {

    HW_CORE_ERROR("Failed load shader file: {}", this->Path());
    this->SetCompiled(ShaderCompileStatus::Failed);
    return false;
  }

  const char *shader_cstr = shader_code.c_str();
  handle_ = glCreateShader(GetType(this->Type()));
  glShaderSource(handle_, 1, &shader_cstr, nullptr);
  glCompileShader(handle_);

  if (CheckError()) {
    this->SetCompiled(ShaderCompileStatus::Success);
    return true;
  }
  this->SetCompiled(ShaderCompileStatus::Failed);
  return false;
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
