#pragma once

#include "GLFW/glfw3.h"
#include "heavy_weather/core/Window.hpp"

namespace weather {

class LinuxWindow : public Window {

public:
  LinuxWindow(const LinuxWindow &) = delete;
  LinuxWindow(LinuxWindow &&) = delete;
  LinuxWindow &operator=(const LinuxWindow &) = delete;
  LinuxWindow &operator=(LinuxWindow &&) = delete;

  LinuxWindow(const WindowProps &props);
  ~LinuxWindow() override;
  void Update() override;
  void Close() override;
  const WindowProps &GetProps() const override;
  void *GetNative() override;

private:
  GLFWwindow *window_;
  WindowProps props_;
};
} // namespace weather
