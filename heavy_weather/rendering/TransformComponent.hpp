#pragma once

#include "heavy_weather/core/Logger.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

namespace weather::graphics {

struct TransformComponent {
  void ComputeMatrix() {
    if (!dirty) {
      return;
    }
    matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, translation);
    matrix = glm::rotate(matrix, glm::radians(rotation.x),
                         glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y),
                         glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z),
                         glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, scale);
    dirty = false;
  }

  bool dirty = false;
  glm::mat4 matrix = glm::mat4{1.0f};
  glm::vec3 rotation{0.0f, 0.0f, 0.0f};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  glm::vec3 translation{0.0f, 0.0f, 0.0f}; // Translation = World Position
};

} // namespace weather::graphics
