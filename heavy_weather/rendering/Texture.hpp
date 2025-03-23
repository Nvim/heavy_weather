#pragma once

#include "heavy_weather/engine.h"
#include <string>

namespace weather::graphics {

enum class TextureWrapFlag : u8 {
  REPEAT,
  REPEAT_MIRROR,
  CLAMP_EDGE,
  CLAMP_BORDER
};

enum class TextureFilterFlag : u8 { LINEAR, NEAREST };

class Texture {
public:
  explicit Texture(std::string path);
  u32 Handle() const;
  const std::string &Path() const;
  const std::string &Name() const;
  bool Loaded() const;

private:
  std::string path_;
  std::string name_;
  bool loaded_{false};
};

} // namespace weather::graphics
