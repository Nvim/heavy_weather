#pragma once

#include "heavy_weather/engine.h"
namespace weather::graphics {
class Texture;

struct TextureComponent {
  SharedPtr<Texture> texture;
};

} // namespace weather::graphics
