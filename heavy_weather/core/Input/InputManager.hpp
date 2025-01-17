#pragma once

#include "heavy_weather/engine.h"
#include <utility>
namespace weather {

// Implementation interface
class InputManager {
public:
  // InputManager(void* window);
  virtual ~InputManager();
  virtual bool isKeyDown(int key) = 0;
  virtual std::pair<f64, f64> getMousePos() = 0;
};

} // namespace weather
