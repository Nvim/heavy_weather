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
  f64 frametime; // Time the frame actually took
  f64 fps; // FPS, counting sleep to meet configured cap
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
  ~Gui();

private:
  void OnRemoveWidget(const WidgetCloseEvent &e); // Unused event handler
  void GarbageCollect(); // Remove all widgets marked for removal
  ImGuiIO *io_ = nullptr;
  void *m_window_ = nullptr;
  std::vector<UniquePtr<IWidget>> widgets_;

  /*
   * Widget removals are processed at the end of frame.
   * This is because they are triggered from the 'Delete' button callback,
   * which will be ran while iterating through the widgets in `Render()`.
   * */
  std::vector<u64> removals_; 
};

} // namespace weather::graphics
