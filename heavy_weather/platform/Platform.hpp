// Window creation & keyboard input abstraction
//
#pragma once

#include "heavy_weather/core/Input/InputManager.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include <memory>

namespace weather {

void PlatformSleep(u64 time);
f64 PlatformGetTime();
std::unique_ptr<Window> PlatformInitWindow(const WindowProps &props);
InputManager *PlatformInitInput(void *window);

} // namespace weather
