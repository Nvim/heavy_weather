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
  const std::string &Name() const { return name_; }
  void Reload();

private:
  std::string source_;
  std::filesystem::path path_;
  std::string name_;
  bool empty_{true};
  bool Read();
};

} // namespace weather
