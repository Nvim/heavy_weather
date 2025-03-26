#pragma once

#include <heavy_weather/rendering/Texture.hpp>

namespace weather::graphics {

class GLTexture : public Texture {
public:
  // explicit GLTexture(const std::string &path);
  explicit GLTexture(const std::string &path, const TextureParams &params = {});
  ~GLTexture() override;

  void Bind() const override;
  void SetMinFilterFlag(TextureFilterFlag flag) override;
  void SetMagFilterFlag(TextureFilterFlag flag) override;
  void SetWrapFlag(TextureWrapFlag flag) override;
  void SetParams(const TextureParams &params) override;

  GLTexture(const GLTexture &) = default;
  GLTexture(GLTexture &&) = delete;
  GLTexture &operator=(const GLTexture &) = default;
  GLTexture &operator=(GLTexture &&) = delete;
};

} // namespace weather::graphics
