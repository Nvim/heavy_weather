#pragma once
#include "InputManager.hpp"
#include "heavy_weather/platform/Platform.hpp"
#include <heavy_weather/core/Asserts.hpp>
#include <heavy_weather/core/Logger.hpp>
#include <heavy_weather/engine.h>

namespace weather {

/**
 * @class InputSystem
 * @brief Act as a proxy to query platform about input status. Can be queried
 * by other systems. Singleton system, coupled with Window. Depends on window
 * having polled the latest events, and doesn't store any input status
 */
class InputSystem {
public:
  static bool Init(void *window) {
    if (!s_impl) {
      HW_ASSERT(window != nullptr);
      HW_CORE_DEBUG("Input manager initialized");
      s_impl = platform_init_input(window);
    }
    return false;
  };
  static bool isKeyDown(int key) {
    HW_ASSERT(s_impl);
    return s_impl->isKeyDown(key);
  }
  static std::pair<f64, f64> getMousePos() { return s_impl->getMousePos(); }

  static void Shutdown() {
    if (s_impl) {
      delete s_impl;
    }
  }

  // TODO: To poll input, we just proxy query to glfw, assuming polling is
  // done by window. We may need input polling part done here instead at some
  // time?
private:
  static inline InputManager *s_impl;
};
} // namespace weather
