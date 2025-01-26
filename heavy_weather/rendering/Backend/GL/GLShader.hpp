#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <heavy_weather/rendering/Shader.hpp>
#include <utility>

namespace weather::graphics {

// Can't constexpr because GL wants ref to variable
#define MAX_SHADER_LOG 4096 // NOLINT

class GLShader : public Shader {
public:
  explicit GLShader(ShaderType type, std::string path)
      : Shader{type, std::move(path)} {}

  bool Compile() override;
  u32 Hanlde() const { return handle_; }

  //
  ~GLShader() override;
  GLShader(const GLShader &) = default;
  GLShader(GLShader &&) = delete;
  GLShader &operator=(const GLShader &) = default;
  GLShader &operator=(GLShader &&) = delete;

private:
  u32 handle_{};
  // Returns false if an error occured:
  bool CheckError() const;
};

} // namespace weather::graphics
