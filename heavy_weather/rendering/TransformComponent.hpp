#pragma once

#include <glm/mat4x4.hpp>

namespace weather::graphics {

struct TransformComponent {
  void ComputeMatrix();

  bool dirty = false;
  glm::mat4 matrix = glm::mat4{1.0f};
  glm::vec3 rotation{0.0f, 0.0f, 0.0f};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};
  glm::vec3 translation{0.0f, 0.0f, 0.0f}; // Translation = World Position
};

} // namespace weather::graphics
