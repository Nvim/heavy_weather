// Window creation & keyboard input abstraction
//
#pragma once

#include "heavy_weather/core/Input/InputManager.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include <memory>

namespace weather {

void platform_sleep(u64 time);
f64 platform_get_time(void);
std::unique_ptr<Window> platform_init_window(s_WindowProps props);
InputManager *platform_init_input(void *window);

} // namespace weather
