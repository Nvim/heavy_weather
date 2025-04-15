#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <heavy_weather/engine.h>

namespace weather::graphics {

struct CameraParams {
  f32 fov{60.0f};
  f32 near{0.1f};
  f32 far{100.0f};
  f32 yaw{-90.0f};
  f32 pitch{0.0f};
  f32 sens{0.1f};
  f32 speed{5.0f};
  glm::vec3 pos{0.0f, 0.0f, 3.0f};
  glm::vec3 up{0.0f, 1.0f, 0.0f};
};

class Camera {
public:
  explicit Camera(CameraParams &params);

  void Touch() { dirty_ = true; }

  // Use angles to update vectors, then compute matrix from vectors
  void Update();
  void ProcessInput(f64 delta);

  const glm::mat4 &GetMatrix() const { return matrix_; }
  const glm::vec3 &Position() const { return world_pos_; }
  f32 Far() const { return far_; }
  f32 Near() const { return near_; }
  f32 Fov() const { return fov_; }

private:
  bool dirty_{true}; // we want to update right after construct to init vecs/mat

  glm::mat4 matrix_{1.0f}; // view matrix

  // Vectors:
  glm::vec3 lookat_{0.0f, 0.0f, 0.0f}; // direction camera is looking at
  glm::vec3 world_pos_;                // position in world space
  glm::vec3 right_;                    // relative to camera
  glm::vec3 up_{0.0f, 1.0f, 0.0f};     // relative to camera
  glm::vec3 world_up_;                 // world space

  // Angles:
  f32 yaw_;
  f32 pitch_;
  f32 fov_;
  f32 far_;
  f32 near_;

  // User controls:
  f32 sens_;
  f32 speed_;
  std::pair<f64, f64> mouse_{0.0f, 0.0f};
  void ProcessKeyboard(f64 delta);
  void ProcessMouse(f64 delta);
};

} // namespace weather::graphics
