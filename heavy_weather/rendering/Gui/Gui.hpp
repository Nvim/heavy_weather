#pragma once

#include "heavy_weather/event/WidgetCloseEvent.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <heavy_weather/rendering/Gui/IWidget.hpp>
#include <vector>

namespace weather::graphics {

struct GuiDesc {
  Backend backend;
  void *window;
};

class Gui {
public:
  Gui(GuiDesc desc);
  void Render();
  u64 AddWidget(UniquePtr<IWidget> w);
  void RemoveWidget(u64 id);
  // void ShowCursor();
  // void HideCursor();

  ImGuiIO *GetIO();
  // ~Gui();

private:
  void OnRemoveWidget(const WidgetCloseEvent &e);
  ImGuiIO *io_ = nullptr;
  void *m_window_ = nullptr;
  std::vector<UniquePtr<IWidget>> widgets_;
};

} // namespace weather::graphics
