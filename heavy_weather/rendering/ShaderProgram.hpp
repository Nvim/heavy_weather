#pragma once

#include "Shader.hpp"
#include "Types.hpp"
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
    HW_ASSERT(desc.FragmentShader.get()->Type() == ShaderType::FragmentShader);
    HW_ASSERT(desc.VertexShader.get()->Type() == ShaderType::VertexShader);
    name_ = desc.VertexShader.get()->Path().stem().string() +
            desc.FragmentShader.get()->Path().stem().string();
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

protected:
  SharedPtr<Shader> vertex_{nullptr};
  SharedPtr<Shader> fragment_{nullptr};

private:
  std::string name_;
};
} // namespace weather::graphics
