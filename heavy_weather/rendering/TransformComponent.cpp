#include "TransformComponent.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>

namespace weather::graphics {
void TransformComponent::ComputeMatrix() {
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
  // matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  // matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  // matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  matrix = glm::scale(matrix, scale);
  dirty = false;
}
} // namespace weather::graphics
