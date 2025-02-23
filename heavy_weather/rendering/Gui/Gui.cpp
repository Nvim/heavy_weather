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
#include "heavy_weather/rendering/Types.hpp"
#include "imgui.h"
#include <algorithm>
#include <functional>
#include <glm/glm.hpp>

static int s_count = 0;

namespace weather::graphics {

Gui::Gui(GuiDesc desc) : m_window_{desc.window} {
  HW_ASSERT_MSG(desc.backend == Backend::OpenGL,
                "Only OpenGL is supported as a GUI backend");
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
  HW_ASSERT_MSG(
      ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow *>(m_window_), true),
      "Faild initializing imgui glfw impl");
  HW_ASSERT(ImGui_ImplOpenGL3_Init("#version 330"));

  EventCallback<WidgetCloseEvent> e =
      std::bind(&Gui::OnRemoveWidget, this, std::placeholders::_1);
  EventRegister(e);
}

void Gui::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  for (auto &c : widgets_) {
    c->Render();
  }

  ImGui::Render();
}

u64 Gui::AddWidget(UniquePtr<IWidget> w) {
  w->SetID(++s_count);
  widgets_.push_back(std::move(w));
  HW_CORE_DEBUG("Added Gui widget #{}", s_count);
  return s_count;
}

void Gui::RemoveWidget(u64 id) { EventDispatch(WidgetCloseEvent{(id)}); }

void Gui::OnRemoveWidget(const WidgetCloseEvent &e) {
  u64 id = e.GetID();
  auto elem = std::find_if(
      widgets_.begin(), widgets_.end(),
      [&id](const UniquePtr<IWidget> &c) { return c->GetID() == id; });
  if (elem != widgets_.end()) {
    HW_CORE_DEBUG("Removing Gui widget #{}", id);
    widgets_.erase(elem);
  } else {
    HW_CORE_DEBUG("Couldn't remove Gui widget #{}", id);
  }
}

} // namespace weather::graphics
