#include "Gui.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/EventCallback.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WidgetCloseEvent.hpp"
#include "heavy_weather/rendering/Gui/GuiComponent.hpp"
#include "imgui.h"
#include <algorithm>
#include <functional>
#include <glm/glm.hpp>
#include <memory>

static int s_count = 0;

namespace weather::graphics {

Gui::Gui(GLFWwindow *window) : m_window_{window} {
  ImGui::CreateContext();
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io_ = &io;
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;           // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  HW_ASSERT_MSG(ImGui_ImplGlfw_InitForOpenGL(window, true),
                "Faild initializing imgui glfw impl");
  HW_ASSERT(ImGui_ImplOpenGL3_Init("#version 330"));

  EventCallback<WidgetCloseEvent> e =
      std::bind(&Gui::OnRemoveComponent, this, std::placeholders::_1);
  EventRegister(e);
}

void Gui::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  for (auto &c : components_) {
    c.Render();
  }

  ImGui::Render();
}

void Gui::AddComponent(GuiComponentDesc &desc) {
  HW_CORE_DEBUG("Adding Gui component #{}", s_count + 1);
  components_.emplace_back(desc, ++s_count);
}

void Gui::RemoveComponent(u64 id) { EventDispatch(WidgetCloseEvent{(id)}); }

void Gui::OnRemoveComponent(const WidgetCloseEvent &e) {
  u64 id = e.GetID();
  auto elem = std::find_if(components_.begin(), components_.end(),
                           [&id](const GuiComponent &c) { return c.id == id; });
  if (elem != components_.end()) {
    HW_CORE_DEBUG("Removing Gui component #{}", id);
    components_.erase(elem);
  } else {
    HW_CORE_DEBUG("Couldn't remove Gui component #{}", id);
  }
}

} // namespace weather::graphics
