#include "Gui.hpp"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "imgui.h"
#include <glm/glm.hpp>

namespace weather::graphics {

void Gui::Init(GuiDesc desc) {
  if (desc.backend != Backend::OpenGL) {
    HW_CORE_ERROR("Only OpenGL is supported as a GUI backend");
    HW_ASSERT(false);
    return;
  }
  ImGui::CreateContext();
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;           // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  HW_ASSERT_MSG(ImGui_ImplGlfw_InitForOpenGL(
                    static_cast<GLFWwindow *>(desc.window), true),
                "Faild initializing imgui glfw impl");
  HW_ASSERT(ImGui_ImplOpenGL3_Init("#version 330"));
}

void Gui::ShutDown() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Gui::RenderAppWindow(AppInfo &info, void *window) {
  if (ImGui::Begin("Application")) {
    ImGui::Text("%s\n%s", info.program_name, info.engine_name);
    ImGui::Separator();
    ImGui::Text("FPS: %4f (%6f ms)", info.fps, info.frametime);
    ImGui::Text("Real frametime: %5f ms", info.real_frametime);
    if (ImGui::Button("Exit")) {
      EventDispatch(weather::WindowCloseEvent{window});
    }
    ImGui::ShowMetricsWindow();
    ImGui::End();
  }
}

void Gui::BeginFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void Gui::EndFrame() {
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

bool Gui::DrawInputFloat(const char *name, void *data) {
  return (ImGui::InputFloat(name, (f32 *)data));
}
bool Gui::DrawInputFloat2(const char *name, void *data) {
  return (ImGui::InputFloat2(name, (f32 *)data));
}
bool Gui::DrawInputFloat3(const char *name, void *data) {
  return (ImGui::InputFloat3(name, (f32 *)data));
}
bool Gui::DrawInputFloat4(const char *name, void *data) {
  return (ImGui::InputFloat4(name, (f32 *)data));
}
bool Gui::DrawScalarInt(const char *name, void *data, i32 step) {
  return (ImGui::InputScalar(name, ImGuiDataType_S32, data, &step));
}
bool Gui::DrawScalarFloat(const char *name, void *data, f32 step) {
  return (ImGui::InputScalar(name, ImGuiDataType_Float, data, &step));
}

} // namespace weather::graphics
