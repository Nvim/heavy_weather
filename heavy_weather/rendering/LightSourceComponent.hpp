#pragma once

#include "heavy_weather/engine.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace weather::graphics {

enum class LightSourceType : u8 {
  DirectionalLight,
  PointLight,
};

struct LightSourceComponent {
  glm::vec3 position{0.0f, 0.0f, 0.0f};
  f32 constant{0.0f};
  glm::vec3 ambient{0.0f, 0.0f, 0.0f};
  f32 linear{0.0f};
  glm::vec3 diffuse{0.0f, 0.0f, 0.0f};
  f32 quadratic{0.0f};
  glm::vec4 specular{0.0f, 0.0f, 0.0f, 0.0f}; // vec4 for padding in arrays
};

} // namespace weather::graphics
