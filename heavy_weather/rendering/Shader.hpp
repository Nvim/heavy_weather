#pragma once

#include <utility>

#include "heavy_weather/rendering/Types.hpp"
namespace weather::graphics {

// Represents a shader file on disk, holds it's shader type,
// Can compile and retrieve errors
class Shader {
public:
  explicit Shader(ShaderType type, std::string path)
      : type_{type}, path_{std::move(path)} {}

  virtual bool Compile() = 0;
  ShaderType Type() const { return type_; }
  ShaderCompileStatus Status() const { return compiled_; }
  const std::string &Path() const { return path_; }

  //
  virtual ~Shader() = default;
  Shader(const Shader &) = default;
  Shader(Shader &&) = delete;
  Shader &operator=(const Shader &) = default;
  Shader &operator=(Shader &&) = delete;

protected:
  void SetCompiled(ShaderCompileStatus status) { compiled_ = status; }

private:
  ShaderType type_;
  std::string path_;
  ShaderCompileStatus compiled_{NotCompiled}; // Has it been compiled yet
};

} // namespace weather::graphics
