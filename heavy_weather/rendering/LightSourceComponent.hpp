#pragma once

#include "heavy_weather/engine.h"
#include <glm/vec3.hpp>

namespace weather::graphics {

enum class LightSourceType : u8 {
  DirectionalLight,
  PointLight,
};

struct LightSourceComponent {
  // position comes from expected TransformComponent
  // TODO: embed TransformComponent for lightsource & regular renderable object
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

} // namespace weather::graphics
