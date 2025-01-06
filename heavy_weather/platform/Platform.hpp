// Window creation & keyboard input abstraction
//
// I'll still include GLFW in client code for input defines, but will make my
// own at some point
#pragma once

#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include <memory>

namespace weather {

#ifdef PLATFORM_LINUX

class LinuxWindow;
void platform_sleep(u64 time);
f64 platform_get_time(void);
std::unique_ptr<Window>
platform_init_window(s_WindowProps props);

#else

class Window;
void platform_sleep(f64 time);
f64 platform_get_time(void);
std::unique_ptr<Window> &&
platform_init_window(s_WindowProps props);

#endif

} // namespace weather
