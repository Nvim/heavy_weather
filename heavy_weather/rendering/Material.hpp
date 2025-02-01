#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <glm/glm.hpp>

namespace weather::graphics {

struct Material {
  glm::vec4 color{0.1f, 0.3f, 0.7f, 1.0f};
  void *Get() { return &color; }
  UniformDescriptor uniform{"iMaterial", DataFormat::Float4, &color};
};

} // namespace weather::graphics
