#pragma once

#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/KeyPressedEvent.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/platform/platform.h"

namespace weather {
class Application {

public:
  Application();
  ~Application();

  void Run();

  void OnKeyPressed(const KeyPressedEvent &evt);
  void OnResize(const ResizeEvent &evt);

private:
  std::unique_ptr<Window> window_;
  bool is_running_;
  EventCallback<KeyPressedEvent> key_callback_;
  EventCallback<ResizeEvent> resize_callback_;
};
} // namespace weather
