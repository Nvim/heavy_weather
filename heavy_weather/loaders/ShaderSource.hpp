#pragma once

#include <filesystem>
#include <string>
namespace weather {

class ShaderSource {
public:
  ShaderSource(const std::filesystem::path &path);
  const std::filesystem::path &Path() const { return path_; }
  const std::string &Data() const { return source_; }
  bool Empty() const { return empty_; }

private:
  std::string source_;
  std::filesystem::path path_;
  bool empty_{true};
};

} // namespace weather
