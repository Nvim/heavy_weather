#pragma once
#include "heavy_weather/engine.h"
#include <filesystem>

namespace weather::graphics {

/* Encapsulate stb_image library in a safer and easier API */
class Image {
public:
  Image();
  explicit Image(const std::filesystem::path &img_path);
  Image(const Image &) = default;
  Image &operator=(const Image &) = default;
  Image &operator=(Image &&) = delete;
  Image(Image &&) = delete;

  ~Image();

  std::pair<u32, u32> Size() const;
  bool Empty() const;
  const std::filesystem::path &Path() const;
  u8 At(u32 row, u32 col) const;
  u8 *Data();
  i32 Channels() const;

private:
  u8 *image_data_;
  std::pair<u32, u32> size_;
  i32 channels_;
  bool empty_;
  std::filesystem::path path_;

  void InitEmpty(std::filesystem::path img_path);
};

} // namespace weather::graphics
