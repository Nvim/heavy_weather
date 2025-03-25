#pragma once

#include <heavy_weather/rendering/Texture.hpp>

namespace weather::graphics {

class GLTexture : public Texture {
public:
  explicit GLTexture(const std::string &path);
  ~GLTexture() override;

  void Bind() const override;
  void SetFilterFlag(TextureFilterFlag flag) override;
  void SetWrapFlag(TextureWrapFlag flag) override;

  GLTexture(const GLTexture &) = default;
  GLTexture(GLTexture &&) = delete;
  GLTexture &operator=(const GLTexture &) = default;
  GLTexture &operator=(GLTexture &&) = delete;
};

} // namespace weather::graphics
