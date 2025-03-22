#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <glm/glm.hpp>

namespace weather::graphics {

struct MaterialComponent {
  glm::vec4 color{0.1f, 0.3f, 0.7f, 1.0f};
  // UniformDescriptor uniform{"iMaterial", DataFormat::Float4, &color};
};

} // namespace weather::graphics
