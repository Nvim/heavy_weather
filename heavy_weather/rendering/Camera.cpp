#include "Camera.hpp"
#include "heavy_weather/core/Input/InputSystem.hpp"
#include "heavy_weather/core/Logger.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <heavy_weather/core/Input/KeyCodes.hpp>

namespace weather::graphics {

Camera::Camera(CameraParams &params)
    : world_pos_{params.pos}, right_{1.0f, 0.0f, 0.0f}, world_up_{params.up},
      yaw_{params.yaw}, pitch_{params.pitch}, fov_{params.fov},
      far_{params.far}, near_{params.near}, sens_{params.sens},
      speed_{params.speed} {
  // Compute internal vectors & matrix:
  mouse_ = InputSystem::GetMousePos();
  Update();
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

void Camera::ProcessInput(f64 delta) {
  ProcessKeyboard(delta);
  // ProcessMouse(delta);
  // No update, vectors can also be updated by GUI. We want 1 update/frame.
}

void Camera::ProcessKeyboard(f64 delta) {
  f32 speed = static_cast<f32>(speed_ * delta);
  if (InputSystem::IsKeyDown(HW_KEY_W)) {
    world_pos_ += (lookat_ * speed);
    dirty_ = true;
  }
  if (InputSystem::IsKeyDown(HW_KEY_S)) {
    world_pos_ -= (lookat_ * speed);
    dirty_ = true;
  }
  if (InputSystem::IsKeyDown(HW_KEY_A)) {
    world_pos_ -= (right_ * speed);
    dirty_ = true;
  }
  if (InputSystem::IsKeyDown(HW_KEY_D)) {
    world_pos_ += (right_ * speed);
    dirty_ = true;
  }

  // Camera
  if (InputSystem::IsKeyDown(HW_KEY_UP)) {
    pitch_ = std::min(pitch_ + 0.05f, 89.0f);
    dirty_ = true;
  }
  if (InputSystem::IsKeyDown(HW_KEY_DOWN)) {
    pitch_ = std::max(pitch_ - 0.05f, -89.0f);
    dirty_ = true;
  }
  if (InputSystem::IsKeyDown(HW_KEY_LEFT)) {
    yaw_ -= 0.05f;
    dirty_ = true;
  }
  if (InputSystem::IsKeyDown(HW_KEY_RIGHT)) {
    yaw_ += 0.05f;
    dirty_ = true;
  }
}

void Camera::ProcessMouse(f64 delta) {
  (void)delta;
  auto offsets = InputSystem::GetMousePos();
  auto x_off = offsets.first - mouse_.first;
  auto y_off = mouse_.second - offsets.second;
  if (x_off == 0.0f && y_off == 0.0f) {
    return;
  }

  x_off *= sens_;
  y_off *= sens_;
  yaw_ += x_off;
  pitch_ += y_off;
  pitch_ = std::clamp(pitch_, -89.0f, 89.0f);
  dirty_ = true;
  mouse_.first = offsets.first;
  mouse_.second = offsets.second;
}

} // namespace weather::graphics
