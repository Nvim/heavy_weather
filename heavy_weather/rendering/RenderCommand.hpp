#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/BackendApi.hpp"
#include <glm/ext/matrix_float4x4.hpp>

namespace weather::graphics {

struct GeometryComponent;
class Material;

class RenderCommand {

public:
  RenderCommand() = default;
  virtual ~RenderCommand() = default;
  virtual bool Execute([[maybe_unused]] BackendAPI &api) = 0;

  RenderCommand(const RenderCommand &) = delete;
  RenderCommand(RenderCommand &&) = delete;
  RenderCommand &operator=(const RenderCommand &) = default;
  RenderCommand &operator=(RenderCommand &&) = delete;
};

namespace rendercommands {

class RenderMeshCmd : public RenderCommand {
public:
  bool Execute([[maybe_unused]] BackendAPI &api) override;
  RenderMeshCmd(GeometryComponent *geometry, Material *material,
                glm::mat4 model);

private:
  glm::mat4 model_;
  GeometryComponent *geometry_;
  Material *material_;
};

} // namespace rendercommands

} // namespace weather::graphics
