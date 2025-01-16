#pragma once

#include "GLFW/glfw3.h"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/platform/Platform.hpp"

namespace weather {

class LinuxWindow : public Window {

public:
  LinuxWindow(const s_WindowProps& props);
  ~LinuxWindow() override;
  void Update() override;
  void Close() override;
  const s_WindowProps& GetProps() const override;
  virtual void* GetNative() override;

private:
  GLFWwindow* window_;
  s_WindowProps props_;

};
} // namespace weather
