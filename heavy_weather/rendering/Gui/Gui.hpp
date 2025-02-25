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

// Data from the base app class, displayed on every example app
struct AppInfo {
  const char *program_name;
  const char *engine_name;
  f64 frametime;
};

class Gui {
public:
  Gui(GuiDesc desc);
  void Render();
  // TODO: these should be private and App should be friend
  void RenderAppWindow(AppInfo& info) const;
  void EndFrame() const;
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
