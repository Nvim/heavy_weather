#pragma once

#include <heavy_weather/core/InputManager.hpp>
namespace weather {

/**
 * @class GLFWInputManager
 * @brief InputManager implementation for glfw.
 *
 */
class GLFWInputManager : public InputManager {
public:
  bool isKeyDown(int key);
  std::pair<f64, f64> getMousePos();
};

} // namespace weather
