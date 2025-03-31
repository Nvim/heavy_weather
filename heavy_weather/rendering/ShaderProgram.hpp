#pragma once

#include "Shader.hpp"
#include "Types.hpp"
#include <filesystem>
#include <heavy_weather/core/Asserts.hpp>
#include <heavy_weather/engine.h>

namespace weather::graphics {

class ShaderProgram {
public:
  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram(ShaderProgram &&) = delete;
  ShaderProgram &operator=(const ShaderProgram &) = delete;
  ShaderProgram &operator=(ShaderProgram &&) = delete;

  explicit ShaderProgram(PipelineDescriptor &desc) {
    auto &vs = *desc.VertexShader.get();
    auto &fs = *desc.FragmentShader.get();
    HW_ASSERT(fs.Type() == ShaderType::FragmentShader);
    HW_ASSERT(vs.Type() == ShaderType::VertexShader);
    name_ = CreateName(vs.Path(), fs.Path());
    vertex_ = std::move(desc.VertexShader);
    fragment_ = std::move(desc.FragmentShader);
  }
  virtual ~ShaderProgram() = default;
  virtual ShaderCompileStatus Init() = 0;

  const Shader &VertexShader() const { return *vertex_; }
  const Shader &FragmentShader() const { return *fragment_; }
  virtual u32 Handle() const = 0;
  const std::string &Name() const { return name_; }

  virtual void BindUniform(UniformDescriptor &desc) = 0;
  static std::string CreateName(const std::filesystem::path &vs,
                                const std::filesystem::path &fs) {
    return vs.stem().string() + fs.stem().string();
  }

protected:
  SharedPtr<Shader> vertex_{nullptr};
  SharedPtr<Shader> fragment_{nullptr};

private:
  std::string name_;
};
} // namespace weather::graphics
