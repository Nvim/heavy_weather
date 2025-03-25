#pragma once

#include "heavy_weather/rendering/Texture.hpp"
namespace weather::graphics {

struct TextureComponent {
  SharedPtr<Texture> texture;
};

} // namespace weather::graphics
