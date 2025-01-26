
#pragma once

#include "Types.hpp"
#include <string>
#include <utility>
#include <vector>

namespace weather::graphics {
using std::pair;
using std::vector;

class VertexLayout {
public:
  VertexLayout() = default;

  void AddAttribute(const pair<std::string, DataFormat> &attrib) {
    attributes_.push_back(attrib);
    stride_ += FormatSize(attrib.second);
  }
  // Number of attributes
  u32 AttribCount() const { return attributes_.size(); }
  // Size of a vertex
  i32 Stride() const { return stride_; }

  std::vector<pair<std::string, DataFormat>>::const_iterator Begin() const {
    return attributes_.begin();
  }

  std::vector<pair<std::string, DataFormat>>::const_iterator End() const {
    return attributes_.end();
  }

  // No reassignment and move as its not needed
  VertexLayout(const VertexLayout &) = default;
  VertexLayout(VertexLayout &&) = delete;
  VertexLayout &operator=(const VertexLayout &) = default;
  VertexLayout &operator=(VertexLayout &&) = delete;
  ~VertexLayout() = default;

private:
  vector<pair<std::string, DataFormat>> attributes_;
  i32 stride_{}; // size in bytes of each vertex
};
} // namespace weather::graphics
