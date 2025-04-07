#include "ShaderSource.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/event/ResourceReloadEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include <fstream>
#include <sstream>

namespace weather {

ShaderSource::ShaderSource(const std::filesystem::path &path) : path_{path} {
  if (Read()) {
    empty_ = false;
  } else {
    empty_ = true;
    HW_CORE_WARN("ShaderSource: Error reading from path {}", path_.c_str());
  };
  name_ = path.string();
}

void ShaderSource::Reload() {
  if (Read()) {
    HW_CORE_DEBUG("ShaderSource {} reloaded successfully", path_.c_str());
    EventDispatch(ResourceReloadEvent<ShaderSource>{this});
  } else {
    HW_CORE_WARN("ShaderSource: Error reloading from path {}", path_.c_str());
  }
}

bool ShaderSource::Read() {
  std::ifstream shader_file;
  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    shader_file.open(this->Path());
    std::stringstream s;

    s << shader_file.rdbuf();
    shader_file.close();

    this->source_ = s.str();
  } catch (std::ifstream::failure &e) {
    HW_CORE_ERROR("Failed to load shader file: {}", this->Path().string());
    return false;
  }
  return true;
}

} // namespace weather
