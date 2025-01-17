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
    if (s_impl == nullptr) {
      HW_ASSERT(window != nullptr);
      HW_CORE_DEBUG("Input manager initialized");
      s_impl = std::unique_ptr<InputManager>(PlatformInitInput(window));
      return true;
    }
    return false;
  };
  static bool IsKeyDown(int key) {
    HW_ASSERT(s_impl);
    return s_impl->IsKeyDown(key);
  }
  static std::pair<f64, f64> GetMousePos() { return s_impl->GetMousePos(); }

  static void Shutdown() {}

  // TODO: To poll input, we just proxy query to glfw, assuming polling is
  // done by window. We may need input polling part done here instead at some
  // time?
private:
  // static inline InputManager *s_impl;
  static inline std::unique_ptr<InputManager> s_impl;
};
} // namespace weather
