#include "VertexLayout.hpp"

namespace weather::graphics {

void VertexLayout::AddAttribute(const pair<std::string, DataFormat> &attrib) {
  attributes_.push_back(attrib);
  stride_ += FormatSize(attrib.second);
}

VertexLayout &VertexLayout::operator=(VertexLayout &&other) noexcept {
  if (this != &other) {
    this->attributes_ = std::move(other.attributes_);
    this->stride_ = other.stride_;
  }
  return *this;
}

VertexLayout::VertexLayout(VertexLayout &&other) noexcept {
  if (this != &other) {
    this->attributes_ = std::move(other.attributes_);
    this->stride_ = other.stride_;
  }
}

} // namespace weather::graphics
