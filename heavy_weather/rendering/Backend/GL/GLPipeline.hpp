#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <heavy_weather/core/Asserts.hpp>
#include <heavy_weather/engine.h>
#include <heavy_weather/rendering/Pipeline.hpp>

namespace weather::graphics {

class GLPipeline : public Pipeline {
public:
  GLPipeline(const GLPipeline &) = delete;
  GLPipeline(GLPipeline &&) = delete;
  GLPipeline &operator=(const GLPipeline &) = delete;
  GLPipeline &operator=(GLPipeline &&) = delete;

  explicit GLPipeline(PipelineDescriptor &desc) : Pipeline(desc) {}
  ~GLPipeline() override = default;
  ShaderCompileStatus Init() override;
  ShaderCompileStatus Status() const { return status_; }
  u32 Handle() const override { return handle_; }

private:
  u32 handle_{};
  ShaderCompileStatus status_{ShaderCompileStatus::NotCompiled};
};
} // namespace weather::graphics
