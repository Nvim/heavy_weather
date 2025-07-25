#pragma once

#include <GLFW/glfw3.h>
#include <heavy_weather/core/Input/InputManager.hpp>
namespace weather {

/**
 * @class GLFWInputManager
 * @brief InputManager implementation for glfw.
 *
 */
class GLFWInputManager : public InputManager {
public:
  GLFWInputManager(void *window);
  bool IsKeyDown(int key) override;
  std::pair<f64, f64> GetMousePos() override;

private:
  GLFWwindow *window_;
};

} // namespace weather
