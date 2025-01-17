#pragma once

#include "heavy_weather/engine.h"
#include <utility>
namespace weather {

// Implementation interface
class InputManager {
public:
  InputManager() = default;
  InputManager(const InputManager &) = delete;
  InputManager(InputManager &&) = delete;
  InputManager &operator=(const InputManager &) = delete;
  InputManager &operator=(InputManager &&) = delete;
  virtual ~InputManager() = default;
  virtual bool IsKeyDown(int key) = 0;
  virtual std::pair<f64, f64> GetMousePos() = 0;
};

} // namespace weather
