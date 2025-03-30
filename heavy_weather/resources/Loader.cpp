#include "Loader.hpp"
#include "Image.hpp"
#include "ShaderSource.hpp"

namespace weather {
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
