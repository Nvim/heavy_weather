#pragma once

#include "Shader.hpp"
#include "Types.hpp"
#include <heavy_weather/core/Asserts.hpp>
#include <heavy_weather/engine.h>

namespace weather::graphics {

class Pipeline {
public:
  Pipeline(const Pipeline &) = delete;
  Pipeline(Pipeline &&) = delete;
  Pipeline &operator=(const Pipeline &) = delete;
  Pipeline &operator=(Pipeline &&) = delete;

  explicit Pipeline(PipelineDescriptor &desc) {
    HW_ASSERT(desc.FragmentShader.get()->Type() == ShaderType::FragmentShader);
    HW_ASSERT(desc.VertexShader.get()->Type() == ShaderType::VertexShader);
    vertex_ = std::move(desc.VertexShader);
    fragment_ = std::move(desc.FragmentShader);
  }
  virtual ~Pipeline() = default;
  virtual ShaderCompileStatus Init() = 0;

  const Shader &VertexShader() const { return *vertex_; }
  const Shader &FragmentShader() const { return *fragment_; }

protected:
  UniquePtr<Shader> vertex_{nullptr};
  UniquePtr<Shader> fragment_{nullptr};
};
} // namespace weather::graphics
