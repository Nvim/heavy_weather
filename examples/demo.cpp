// clang-format off
// #include <glad/glad.h>
// clang-format on
#include "Demo.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Entry.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/rendering/Camera.hpp"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Texture.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "heavy_weather/scene/SceneManager.hpp"
#include <glm/fwd.hpp>
#include <resources/cube_vertices.hpp>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <thirdparty/tinygltf/tiny_gltf.h>

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
    Backend::OpenGL, {kWidth, kHeight}, true, true};

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
  f32 vertices_2[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, //
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, //
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f  //
  };

  u32 square_indices[] = {0, 1, 2, 1, 2, 3};

  // Buffers:
  graphics::VertexLayout sq_layout{};
  sq_layout.AddAttribute({"position", graphics::DataFormat::Float3});
  sq_layout.AddAttribute({"uvs", graphics::DataFormat::Float2});

  graphics::MeshDescriptor sq_desc{
      std::pair(vertices_2, sizeof(vertices_2)),
      std::pair(square_indices, sizeof(square_indices)), &sq_layout, "square"};

  graphics::VertexLayout cube_layout{};
  cube_layout.AddAttribute({"position", graphics::DataFormat::Float3});

  graphics::MeshDescriptor cube_desc{
      std::pair(cube_verts, sizeof(cube_verts)),
      std::pair(cube_indices, sizeof(cube_indices)), &cube_layout, "cube"};

  // Shaders:
  graphics::ShaderDescriptor vsdesc{graphics::ShaderType::VertexShader,
                                    "examples/resources/demo.vert"};
  graphics::ShaderDescriptor fsdesc{graphics::ShaderType::FragmentShader,
                                    "examples/resources/demo.frag"};
  graphics::ShaderDescriptor fade_fsdesc{graphics::ShaderType::FragmentShader,
                                         "examples/resources/demo_fade.frag"};

  auto tex_shader = renderer_.CreatePipeline(vsdesc, fsdesc);
  auto fade_shader = renderer_.CreatePipeline(vsdesc, fade_fsdesc);

  // Texture:
  tex_ = renderer_.CreateTexture("examples/resources/textures/container.png");
  tex_2 = renderer_.CreateTexture("examples/resources/textures/grass.png");
  HW_ASSERT(tex_->Unit() == 0);
  HW_ASSERT(tex_2->Unit() == 1);

  // Materials:
  auto fade_material = std::make_shared<graphics::Material>(fade_shader);
  auto texture_material = std::make_shared<graphics::Material>(tex_shader);

  fade_material->SetUniformValue<glm::vec4>("uMaterial",
                                            glm::vec4{0.1f, 0.3f, 0.7f, 1.0f});
  fade_material->SetUniformValue<i32>("uFlag", 1);

  texture_material->SetUniformValue<SharedPtr<graphics::Texture>>("uTexture",
                                                                  tex_);
  texture_material->SetUniformValue<SharedPtr<graphics::Texture>>("uTexture2",
                                                                  tex_2);
  texture_material->SetUniformValue<f32>("uBlendFactor", 0.5f);

  u32 cube_mesh =
      scene_manager_.AddMesh(cube_desc, glm::vec3{-1.0f, 0.0f, 0.0f});
  u32 square_mesh =
      scene_manager_.AddMesh(sq_desc, glm::vec3{1.0f, 0.0f, 0.0f});

  scene_manager_.AddMaterial(fade_material, cube_mesh);
  scene_manager_.AddMaterial(texture_material, square_mesh);

  {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    bool ret = loader.LoadBinaryFromFile(
        &model, &err, &warn,
        "examples/resources/models/Box.glb"); // for binary glTF(.glb)

    if (!warn.empty()) {
      HW_APP_WARN("WARNING FROM GLTF: {}", warn);
    }

    if (!err.empty()) {
      HW_APP_ERROR("ERROR FROM GLTF: {}", err);
    }

    if (!ret) {
      HW_APP_ERROR("Failed to parse GLTF");
    }
    HW_APP_INFO("GLTF parsing works :)")
  }
}

void Demo::OnRender(f64 delta) {
  scene_manager_.Update(delta);
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
