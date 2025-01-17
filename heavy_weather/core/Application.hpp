#pragma once

#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"

namespace weather {
class Application {

public:
  Application();
  Application(const Application &) = delete;
  Application(Application &&) = delete;
  Application &operator=(const Application &) = delete;
  Application &operator=(Application &&) = delete;
  virtual ~Application();

  void Run();

  // void OnKeyPressed(const KeyPressedEvent &evt);
  void OnResize(const ResizeEvent &evt);
  void OnClose(const WindowCloseEvent &evt);

  static Application &Get() { return *s_instance; }

  const Window &GetWindow() const;

private:
  static inline Application *s_instance{nullptr};
  std::unique_ptr<Window> window_;
  bool is_running_;
  // EventCallback<KeyPressedEvent> key_callback_;
  EventCallback<ResizeEvent> resize_callback_;
  EventCallback<WindowCloseEvent> close_callback_;
};
} // namespace weather
