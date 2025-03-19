// clang-format off
// #include <glad/glad.h>
// clang-format on
#include "Demo.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Entry.hpp"
#include "heavy_weather/core/Input/InputSystem.hpp"
#include "heavy_weather/core/Input/KeyCodes.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/rendering/Camera.hpp"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <functional>
#include <resources/cube_vertices.hpp>

// Shortcut for registering events
#define BIND_EVENT_FUNC(func) std::bind(func, this, std::placeholders::_1)

using weather::graphics::Backend;

/**************
 *   Globals  *
 **************/
namespace {
const std::string kTitle = "Sandbox";
constexpr f64 kFrametime = 1.0f / 60;
constexpr u16 kWidth = 1280;
constexpr u16 kHeight = 720;

weather::graphics::RendererInitParams renderer_init{
    Backend::OpenGL, {kWidth, kHeight}, false, true};

weather::graphics::CameraParams camera_params{};
} // namespace

using namespace weather;

// Entrypoint hook:
weather::Application *weather::CreateAppHook() {
  weather::WindowProps props = {kTitle, kWidth, kHeight};
  return new Demo{props, kFrametime, renderer_init}; // NOLINT
}

/*****************
 *   Demo Funcs  *
 *****************/

Demo::Demo(WindowProps &window_props, f64 fps,
           graphics::RendererInitParams &render_params)
    : Application(window_props, fps), renderer_{render_params},
#ifdef HW_ENABLE_GUI
      gui_{{graphics::Backend::OpenGL, this->GetWindow().GetNative()}},
#endif
      scene_manager_{
          renderer_,
#ifdef HW_ENABLE_GUI
          gui_,
#endif
          camera_params,
      } //
{
  // Event callbacks:
  mouse_callback_ = [this](const MouseMovedEvent &e) { this->OnMouseMoved(e); };
  EventRegister(mouse_callback_);
  EventCallback<KeyPressedEvent> k = BIND_EVENT_FUNC(&Demo::OnKeyPressed);
  EventRegister(k);
  EventCallback<ResizeEvent> r = BIND_EVENT_FUNC(&Demo::OnResize);
  EventRegister(r);

  // Graphics:
  InitGraphics();
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
      -0.5f, -0.5f, 0.0f, //
      0.5f,  -0.5f, 0.0f, //
      0.0f,  0.5f,  0.0f  //
  };
  f32 vertices_2[] = {
      0.0f,   0.25f,  0.0f, //
      0.0f,   -0.25f, 0.0f, //
      -0.25f, 0.0f,   0.0f, //
      0.25f,  0.0f,   0.0f  //
  };

  u32 indices[] = {0, 1, 2}; // NOLINT
  u32 square_indices[] = {0, 1, 3, 2, 1, 0};

  // Buffers:
  graphics::VertexLayout layout{};
  layout.AddAttribute({"position", graphics::DataFormat::Float3});
  // layout.AddAttribute({"color", graphics::DataFormat::Float4});

  graphics::MeshDescriptor mesh_desc{std::pair(vertices_1, sizeof(vertices_1)),
                                     std::pair(indices, sizeof(indices)),
                                     &layout, "triangle"};

  graphics::MeshDescriptor mesh_desc2{
      std::pair(vertices_2, sizeof(vertices_2)),
      std::pair(square_indices, sizeof(square_indices)), &layout, "square"};

  graphics::MeshDescriptor cube_mesh{
      std::pair(cube_verts, sizeof(cube_verts)),
      std::pair(cube_indices, sizeof(cube_indices)), &layout, "cube"};

  // Shaders:
  graphics::ShaderDescriptor vsdesc{graphics::ShaderType::VertexShader,
                                    "demo.vert"};
  graphics::ShaderDescriptor fsdesc{graphics::ShaderType::FragmentShader,
                                    "demo.frag"};

  scene_manager_.AddMesh(mesh_desc2);
  scene_manager_.AddMesh(cube_mesh);
  scene_manager_.AddMesh(mesh_desc);
  pipeline_ = renderer_.CreatePipeline(vsdesc, fsdesc);
}

void Demo::OnRender(f64 delta) {
  scene_manager_.Update(delta);
  // auto time = PlatformGetTime();
  renderer_.UsePipeline(*pipeline_);
  // int loc = glGetUniformLocation(pipeline_->Handle(), "iGlobalTime");
  // glUniform1f(loc, time);
  renderer_.Clear(bgcolor_);
  scene_manager_.SubmitAll();
}

void Demo::OnMouseMoved(const MouseMovedEvent &e) // NOLINT
{
  (void)e;
  // HW_APP_DEBUG("Mouse moved: ({}, {})", e.Position().x, e.Position().y);
}

void Demo::OnKeyPressed(const KeyPressedEvent &evt) {
  i32 action = evt.Action();
  i32 key = evt.KeyCode();
  if (action == GLFW_PRESS) {
    HW_CORE_TRACE("Key {} has been pressed", key);
    // if (weather::InputSystem::IsKeyDown(HW_KEY_O)) {
    //   gui_.RemoveWidget(1);
    // }
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

#ifdef HW_ENABLE_GUI
const graphics::Gui &Demo::GetGui() const { return gui_; }
#endif

const char *Demo::GetProgramName() const { return kTitle.c_str(); }
