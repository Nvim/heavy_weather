// clang-format off
#include <glad/glad.h>
// clang-format on
#include "Demo.hpp"
#include "backends/imgui_impl_opengl3.h"
#include "heavy_weather/core/Entry.hpp"
#include "heavy_weather/core/Input/InputSystem.hpp"
#include "heavy_weather/core/Input/KeyCodes.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "imgui.h"
#include <functional>

// Shortcut for registering events
#define BIND_EVENT_FUNC(func) std::bind(func, this, std::placeholders::_1)

/**************
 *   Globals  *
 **************/
namespace {
const std::string kTitle = "Sandbox";
constexpr f64 kFrametime = 1.0f / 60;
constexpr u16 kWidth = 1280;
constexpr u16 kHeight = 720;
} // namespace

using namespace weather;

// Entrypoint hook:
weather::Application *weather::CreateAppHook() {
  weather::WindowProps props = {kTitle, kWidth, kHeight};
  return new Demo{props, kFrametime};
}

/*****************
 *   Demo Funcs  *
 *****************/
Demo::Demo(WindowProps &window_props, f64 fps)
    : Application(window_props, fps), renderer_{graphics::Backend::OpenGL},
      gui_{static_cast<GLFWwindow *>(this->GetWindow().GetNative())} {
  mouse_callback_ = [this](const MouseMovedEvent &e) { this->OnMouseMoved(e); };
  EventRegister(mouse_callback_);
  EventCallback<KeyPressedEvent> k = BIND_EVENT_FUNC(&Demo::OnKeyPressed);
  EventRegister(k);
  EventCallback<ResizeEvent> r = BIND_EVENT_FUNC(&Demo::OnResize);
  EventRegister(r);

  // Graphics:
  InitGraphics();

  graphics::GuiComponentDesc desc = {graphics::GuiComponentType::Color3Piker,
                                     (void *)(&bgcolor_), 0.0f, 10.0f,
                                     "background"};
  gui_.AddComponent(desc);
}

Demo::~Demo() {
  EventUnregister(mouse_callback_);
  EventCallback<KeyPressedEvent> e = BIND_EVENT_FUNC(&Demo::OnKeyPressed);
  EventUnregister(e);
  EventCallback<ResizeEvent> r = BIND_EVENT_FUNC(&Demo::OnResize);
  EventUnregister(r);
}

void Demo::InitGraphics() {
  f32 vertices_1[] = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f  //
  };
  f32 vertices_2[] = {
      -0.8f,  0.5f,  0.0f, 0.6f, 0.8f, 0.3f, 1.0f, //
      -0.8f,  0.8f,  0.0f, 0.2f, 0.6f, 0.4f, 1.0f, //
      -0.95f, 0.65f, 0.0f, 0.9f, 0.3f, 0.1f, 1.0f, //
      -0.65f, 0.65f, 0.0f, 0.1f, 0.5f, 0.8f, 1.0f  //
  };

  u32 indices[] = {0, 1, 2}; // NOLINT
  u32 square_indices[] = {0, 1, 3, 2, 1, 0};

  // Buffers:
  graphics::VertexLayout layout{};
  layout.AddAttribute({"position", graphics::DataFormat::Float3});
  layout.AddAttribute({"color", graphics::DataFormat::Float4});

  graphics::MeshDescriptor mesh_desc{std::pair(vertices_1, sizeof(vertices_1)),
                                     std::pair(indices, sizeof(indices)),
                                     &layout};

  graphics::MeshDescriptor mesh_desc2{
      std::pair(vertices_2, sizeof(vertices_2)),
      std::pair(square_indices, sizeof(square_indices)), &layout};

  // Shaders:
  graphics::ShaderDescriptor vsdesc{graphics::ShaderType::VertexShader,
                                    "demo.vert"};
  graphics::ShaderDescriptor fsdesc{graphics::ShaderType::FragmentShader,
                                    "demo.frag"};

  tri_ = renderer_.CreateMesh(mesh_desc);
  square_ = renderer_.CreateMesh(mesh_desc2);
  pipeline_ = renderer_.CreatePipeline(vsdesc, fsdesc);
}

void Demo::OnRender(f64 delta) {
  (void)delta;
  auto time = PlatformGetTime();
  renderer_.UsePipeline(*pipeline_);
  int loc = glGetUniformLocation(pipeline_->Handle(), "iGlobalTime");
  glUniform1f(loc, time);
  renderer_.Clear(bgcolor_);
  renderer_.Submit(*tri_);
  renderer_.Submit(*square_);

  gui_.Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Demo::OnMouseMoved(const MouseMovedEvent &e) // NOLINT
{
  HW_APP_DEBUG("Mouse moved: ({}, {})", e.Position().x, e.Position().y);
}

void Demo::OnKeyPressed(const KeyPressedEvent &evt) {
  i32 action = evt.Action();
  i32 key = evt.KeyCode();
  if (action == GLFW_PRESS) {
    HW_CORE_TRACE("Key {} has been pressed", key);
    if (weather::InputSystem::IsKeyDown(HW_KEY_O)) {
      gui_.RemoveComponent(1);
    }
  } else {
    HW_CORE_TRACE("Key {} has been released", key);
  }
  if (key == GLFW_KEY_ESCAPE) {
    HW_CORE_DEBUG("Shutdown keybind has been pressed.");
    EventDispatch(weather::WindowCloseEvent{(void *)&this->GetWindow()});
  }
}

void Demo::OnResize(const ResizeEvent &e) {
  renderer_.Resize({e.NewSize().w, e.NewSize().h});
}
