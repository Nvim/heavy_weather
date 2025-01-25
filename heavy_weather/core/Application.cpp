#include "Application.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Input/InputSystem.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/platform/Platform.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/VertexLayout.hpp"
#include <heavy_weather/rendering/Backend/GL/GLAPI.hpp>
#include <heavy_weather/rendering/Pipeline.hpp>
#include <heavy_weather/rendering/Shader.hpp>
#include <heavy_weather/rendering/Types.hpp>

namespace {
std::string s_title = "Sandbox";
constexpr f64 kFrametime = 1.0f / 60;
} // namespace

namespace weather {

Application::Application() {
  HW_ASSERT_MSG(!s_instance, "App already exists");
  s_instance = this;
  is_running_ = false;
  WindowProps props{s_title, 1280, 720};
  window_ = PlatformInitWindow(props);

  if (!InputSystem::Init(window_->GetNative())) {
    HW_CORE_CRITICAL("Failed to init input");
  }
  resize_callback_ = [this](const ResizeEvent &e) { this->OnResize(e); };
  close_callback_ = [this](const WindowCloseEvent &e) { this->OnClose(e); };
  EventRegister(resize_callback_);
  EventRegister(close_callback_);

  HW_CORE_INFO("App started. W: {}, H: {}", window_->GetProps().width,
               window_->GetProps().height);
}

Application::~Application() {
  EventUnregister(resize_callback_);
  EventUnregister(close_callback_);
  EventSystem::Shutdown();
  HW_CORE_INFO("Application shutting down");
}

const Window &Application::GetWindow() const { return *window_; }

void Application::Run() {
  HW_CORE_INFO("App running");
  is_running_ = true;
  f64 start{}, end{}, remaining{}, delta{};

  // Graphics:
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

  graphics::Renderer renderer{graphics::Backend::OpenGL};
  UniquePtr<graphics::Mesh> mesh = renderer.CreateMesh(mesh_desc);
  UniquePtr<graphics::Mesh> mesh2 = renderer.CreateMesh(mesh_desc2);
  auto pipeline = renderer.CreatePipeline(vsdesc, fsdesc);

  while (is_running_) {
    start = PlatformGetTime();

    window_->Update();
    renderer.UsePipeline(*pipeline);
    renderer.Clear();
    renderer.Submit(*mesh);
    renderer.Submit(*mesh2);
    end = PlatformGetTime();
    delta = end - start;
    remaining = kFrametime - delta;
    if (delta > 0.0f && remaining > 0.0f) {
      PlatformSleep(remaining - 1); // NOLINT
    }
  }
}

void Application::OnResize(const ResizeEvent &evt) // NOLINT
{
  HW_CORE_INFO("App recieved ResizeEvent ({}, {}) => ({}, {})", evt.OldSize().w,
               evt.OldSize().h, evt.NewSize().w, evt.NewSize().h);
}

void Application::OnClose(const WindowCloseEvent &evt) {
  (void)evt;
  HW_CORE_INFO("App recieved WindowClosedEvent");
  window_->Close();
  is_running_ = false;
}
} // namespace weather
