#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace weather::graphics {

Camera::Camera(CameraParams &params)
    : world_pos_{params.pos}, right_{1.0f, 0.0f, 0.0f}, world_up_{params.up},
      yaw_{params.yaw}, pitch_{params.pitch}, fov_{params.fov},
      far_{params.far}, near_{params.near} {
  // Compute internal vectors & matrix:
  this->Update();
}

void Camera::Update() {
  if (!dirty_) {
    return;
  }
  glm::vec3 front{
      cos(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
      sin(glm::radians(pitch_)),
      sin(glm::radians(yaw_)) * cos(glm::radians(pitch_)),
  };

  lookat_ = glm::normalize(front);
  right_ = glm::normalize(glm::cross(lookat_, world_up_));
  up_ = glm::normalize(glm::cross(right_, lookat_));

  matrix_ = glm::lookAt(world_pos_, world_pos_ + lookat_, up_);
  dirty_ = false;
}

} // namespace weather::graphics
