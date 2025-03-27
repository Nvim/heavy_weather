#define STB_IMAGE_IMPLEMENTATION
#include "Image.hpp"
#include "heavy_weather/core/Logger.hpp"
#include <filesystem>
#include <stb/stb_image.h>
#include <utility>

namespace weather::graphics {
Image::Image() : image_data_{}, channels_{}, empty_{true} {}

Image::Image(const std::filesystem::path &img_path) {
  stbi_set_flip_vertically_on_load(true);
  if (!std::filesystem::exists(img_path)) {
    HW_CORE_ERROR("Image file can't be found");
    InitEmpty(img_path);
  } else {
    i32 width{}, height{}, channels{};
    u8 *data = stbi_load(img_path.c_str(), &width, &height, &channels, 0);
    if (!data) {
      HW_CORE_ERROR("Image couldn't be loaded");
      InitEmpty(img_path);
    } else {
      image_data_ = data;
      size_ = {width, height};
      path_ = img_path;
      empty_ = false;
      channels_ = channels;
      HW_CORE_INFO("Image {} loaded. Width: {}, Height: {}, Channels: {}\n",
                   path_.c_str(), size_.first, size_.second, channels_);
    }
  }
}

std::pair<u32, u32> Image::Size() const { return size_; }

i32 Image::Channels() const { return channels_; }

bool Image::Empty() const { return empty_; }

u8 Image::At(u32 row, u32 col) const {
  // if (empty_ || channels_ < 3 || channels_ > 4) {
  // return ftxui::Color{};
  // }
  // y lines of:
  // [00000000 00000000 00000000][00000000 00000000 00000000] ....
  // each square bracket is one x
  // const auto index{channels_ * (row * size_.col + col)};
  // unsigned char *pixel_data = image_data_;
  // return ftxui::Color(pixel_data[index], pixel_data[index + 1],
  //                     pixel_data[index + 2]);
  return 8; // TODO
}

const std::filesystem::path &Image::Path() const { return path_; }

void Image::InitEmpty(std::filesystem::path img_path) {
  image_data_ = nullptr;
  size_ = {0, 0};
  empty_ = true;
  channels_ = 0;
  path_ = std::move(img_path);
}

u8 *Image::Data() { return image_data_; }

Image::~Image() {
  if (!empty_) {
    stbi_image_free(image_data_);
  }
}

} // namespace weather::graphics
