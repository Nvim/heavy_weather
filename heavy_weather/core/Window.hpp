#pragma once

#include "heavy_weather/engine.h"
#include <memory>
#include <string>

namespace weather {

typedef struct s_WindowProps {
  const std::string &title;
  u16 width{0};
  u16 height{0};

  s_WindowProps(const std::string &title = "Untitled", u16 width = 800,
                u16 height = 600)
      : title(title), width(width), height(height) {}
} s_WindowProps;

class Window {

public:
  virtual ~Window() = default;
  virtual const s_WindowProps &GetProps() const = 0;
  virtual void Update() = 0;
  static std::unique_ptr<Window> Create(const s_WindowProps &props);
  virtual void Close() = 0;
};
} // namespace weather
