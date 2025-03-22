#include "Gui.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "imgui.h"
#include <glm/glm.hpp>

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
}

void Gui::RenderAppWindow(AppInfo &info) const {
  if (ImGui::Begin("Application")) {
    ImGui::Text("%s\n%s", info.program_name, info.engine_name);
    ImGui::Separator();
    ImGui::Text("FPS: %5f (%8f ms)", (1.0f / info.fps), info.frametime);
    ImGui::End();
  }
}

void Gui::BeginFrame() const {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Gui::EndFrame() const {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Gui::BeginWindow(const char *title) { return ImGui::Begin(title); }
void Gui::EndWindow() { ImGui::End(); }

bool Gui::BeginTreeNode(const char *title) { return ImGui::TreeNode(title); }
bool Gui::BeginTreeNode() { return true; }
void Gui::EndTreeNode() { ImGui::TreePop(); }

bool Gui::DrawButton(const char *name) { return (ImGui::Button(name)); }
bool Gui::DrawSliderFloat(const char *name, void *data, f32 min, f32 max) {
  return (ImGui::SliderFloat(name, (f32 *)data, min, max));
}
bool Gui::DrawSliderFloat2(const char *name, void *data, f32 min, f32 max) {
  return (ImGui::SliderFloat2(name, (f32 *)data, min, max));
}
bool Gui::DrawSliderFloat3(const char *name, void *data, f32 min, f32 max) {
  return (ImGui::SliderFloat3(name, (f32 *)data, min, max));
}
bool Gui::DrawSliderFloat4(const char *name, void *data, f32 min, f32 max) {
  return (ImGui::SliderFloat4(name, (f32 *)data, min, max));
}
bool Gui::DrawColorEdit4(const char *name, void *data) {
  return (ImGui::ColorEdit4(name, (f32 *)data));
}

Gui::~Gui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

} // namespace weather::graphics
