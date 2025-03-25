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
  Texture(const std::string &path) : path_{path}, unit_{counter++} {};
  virtual ~Texture() = default;

  virtual void Bind() const = 0;
  virtual void SetFilterFlag(TextureFilterFlag flag) = 0;
  virtual void SetWrapFlag(TextureWrapFlag flag) = 0;

  i32 Unit() const { return unit_; }
  bool Loaded() const { return loaded_; }
  const std::string &Path() const { return path_; }
  const std::string &Name() const { return name_; }

  void SetName(const std::string &name) { name_ = name; }

private:
  std::string path_;
  std::string name_;
  bool loaded_{false};
  u32 handle_{0};
  i32 unit_;
  static inline i32 counter{0};

protected:
  u32 Handle() const { return handle_; }
  u32 *HandlePtr() { return &handle_; }
};

} // namespace weather::graphics
