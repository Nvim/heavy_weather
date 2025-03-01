#pragma once

#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Types.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

namespace weather::graphics {

struct Transform {
  void Touch() {
    HW_CORE_DEBUG("Matrix got touched");
    dirty_ = true;
  }
  void ComputeMatrix() {
    if (!dirty_) {
      return;
    }
    matrix_ = glm::mat4(1.0f);
    matrix_ = glm::translate(matrix_, translation_);
    matrix_ = glm::rotate(matrix_, glm::radians(rotation_),
                          glm::vec3(0.0f, 1.0f, 0.0f));
    matrix_ = glm::scale(matrix_, scale_);
    HW_CORE_DEBUG("Translation is now ({}, {}, {}).", translation_.x, translation_.y, translation_.z);
    dirty_ = false;
  }

  const glm::mat4& GetMatrix() const { return matrix_; }

  void *Translation() { return &translation_; }
  void *Scale() { return &scale_; }
  void *Rotation() { return &rotation_; }

private:
  bool dirty_ = false;
  glm::mat4 matrix_ = glm::mat4{1.0f};
  f32 rotation_{0.0f};
  glm::vec3 scale_{1.0f, 1.0f, 1.0f};
  glm::vec3 translation_{0.0f, 0.0f, 0.0f}; // Translation = World Position
};

} // namespace weather::graphics
