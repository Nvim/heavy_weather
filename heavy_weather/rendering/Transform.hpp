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
  bool dirty = false;
  void ComputeMatrix() {
    if (!dirty) {
      return;
    }
    matrix_ = glm::mat4(1.0f);
    matrix_ = glm::scale(matrix_, scale_);
    matrix_ = glm::rotate(matrix_, glm::radians(rotation_),
                          glm::vec3(0.0f, 0.0f, 1.0f));
    matrix_ = glm::translate(matrix_, translation_);
    dirty = false;
  }

  void *Translation() { return &translation_; }
  void *Scale() { return &scale_; }
  void *Rotation() { return &rotation_; }
  UniformDescriptor uniform{"iTransform", DataFormat::Mat4, &matrix_};

private:
  glm::mat4 matrix_ = glm::mat4{1.0f};
  glm::vec3 scale_{1.0f, 1.0f, 1.0f};
  glm::vec3 translation_{0.0f, 0.0f, 0.0f};
  f32 rotation_{0.0f};
};

} // namespace weather::graphics
