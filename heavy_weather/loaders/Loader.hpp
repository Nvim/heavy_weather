#pragma once

#include "Image.hpp"
#include "ShaderSource.hpp"
#include "heavy_weather/engine.h"
#include <filesystem>

namespace weather {

template <typename T> class Loader {
public:
  static SharedPtr<T> Load(const std::filesystem::path &path);
};

template <>
SharedPtr<Image> Loader<Image>::Load(const std::filesystem::path &path) {
  return std::make_shared<Image>(path);
}

template <>
SharedPtr<ShaderSource>
Loader<ShaderSource>::Load(const std::filesystem::path &path) {
  return std::make_shared<ShaderSource>(path);
}

} // namespace weather
