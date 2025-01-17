#pragma once

#include "heavy_weather/engine.h"
#include <memory>
#include <string>

namespace weather {

using WindowProps = struct WindowProps {
  std::string title;
  u16 width{0};
  u16 height{0};

  WindowProps(const std::string &title = "Untitled",
              u16 width = 800,  // NOLINT
              u16 height = 600) // NOLINT
      : title(title), width(width), height(height) {}
};

class Window {

public:
  Window() = default;
  Window(const Window &) = delete;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&) = delete;
  virtual ~Window() = default;
  virtual void *GetNative() = 0;
  virtual const WindowProps &GetProps() const = 0;
  virtual void Update() = 0;
  static std::unique_ptr<Window> Create(const WindowProps &props);
  virtual void Close() = 0;
};
} // namespace weather
