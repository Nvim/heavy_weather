#pragma once

#include "heavy_weather/engine.h"
#include <filesystem>

namespace weather {

template <typename T> class Loader {
public:
  static SharedPtr<T> Load(const std::filesystem::path &path);
};

} // namespace weather
