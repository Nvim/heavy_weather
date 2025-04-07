#pragma once

#include "heavy_weather/event/ResourceReloadEvent.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <heavy_weather/core/Asserts.hpp>
#include <heavy_weather/engine.h>
#include <heavy_weather/rendering/ShaderProgram.hpp>

namespace weather::graphics {

class GLShaderProgram : public ShaderProgram {
public:
  GLShaderProgram(const GLShaderProgram &) = delete;
  GLShaderProgram(GLShaderProgram &&) = delete;
  GLShaderProgram &operator=(const GLShaderProgram &) = delete;
  GLShaderProgram &operator=(GLShaderProgram &&) = delete;

  explicit GLShaderProgram(PipelineDescriptor &desc) : ShaderProgram(desc) {}
  ~GLShaderProgram() override = default;
  ShaderCompileStatus Init() override;
  ShaderCompileStatus Status() const { return status_; }
  u32 Handle() const override { return handle_; }

  void BindUniform(UniformDescriptor &desc) override;

  void Reload() override;
  void OnResourceReload(const ResourceReloadEvent<Shader> &evt) override;

private:
  u8 waiting_{0}; // ResourceReloadEvent<Shader> needed to trigger a re-linking
  u32 handle_{};
  ShaderCompileStatus status_{ShaderCompileStatus::NotCompiled};

  void LinkAndValidate();
};
} // namespace weather::graphics
