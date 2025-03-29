#include "ShaderSource.hpp"
#include "heavy_weather/core/Logger.hpp"
#include <fstream>
#include <sstream>

namespace weather {

ShaderSource::ShaderSource(const std::filesystem::path &path) : path_{path} {
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
  }
  empty_ = false;
}

} // namespace weather
