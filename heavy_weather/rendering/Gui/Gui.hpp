#pragma once

#include "heavy_weather/event/WidgetCloseEvent.hpp"
#include "heavy_weather/rendering/Gui/GuiComponent.hpp"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace weather::graphics {

class Gui {
public:
  Gui(GLFWwindow *window);
  void Render();
  void AddComponent(GuiComponentDesc &desc);
  void RemoveComponent(u64 id);
  // void ShowCursor();
  // void HideCursor();

  ImGuiIO *GetIO();
  // ~Gui();

private:
  void OnRemoveComponent(const WidgetCloseEvent &e);
  ImGuiIO *io_ = nullptr;
  GLFWwindow *m_window_ = nullptr;
  std::vector<GuiComponent> components_;
};

} // namespace weather::graphics
