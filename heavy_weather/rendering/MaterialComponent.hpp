#pragma once

#include "heavy_weather/engine.h"

namespace weather::graphics {
class Material;

struct MaterialComponent {
  SharedPtr<Material> material;
};

} // namespace weather::graphics
