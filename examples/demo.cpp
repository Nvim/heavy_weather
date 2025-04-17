// clang-format off
// #include <glad/glad.h>
// clang-format on
#include "Demo.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Entry.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/GuiRenderEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/rendering/Camera.hpp"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Texture.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "heavy_weather/rendering/VertexLayout.hpp"
#include "heavy_weather/resources/AssetManager.hpp"
#include "heavy_weather/scene/Scene.hpp"
#include "heavy_weather/scene/components/RotateComponent.hpp"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <resources/cube_vertices.hpp>
// #ifndef TINYGLTF_IMPLEMENTATION
// #define TINYGLTF_IMPLEMENTATION
// #endif // !TINYGLTF_IMPLEMENTATION
// #ifndef STB_IMAGE_WRITE_IMPLEMENTATION
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #endif // !STB_IMAGE_WRITE_IMPLEMENTATION
// #ifndef STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #endif // !STB_IMAGE_IMPLEMENTATION
// #include <thirdparty/tinygltf/tiny_gltf.h>

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
      scene_{renderer_, camera_params} //
{
  // Event callbacks:
  mouse_callback_ = [this](const MouseMovedEvent &e) { this->OnMouseMoved(e); };
  EventRegister(mouse_callback_, this);
  EventCallback<KeyPressedEvent> k = BIND_EVENT_FUNC(&Demo::OnKeyPressed);
  EventRegister(k, this);
  EventCallback<ResizeEvent> r = BIND_EVENT_FUNC(&Demo::OnResize);
  EventRegister(r, this);

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
  graphics::VertexLayout cube_layout{};
  cube_layout.AddAttribute({"position", graphics::DataFormat::Float3});
  cube_layout.AddAttribute({"normal", graphics::DataFormat::Float3});
  cube_layout.AddAttribute({"uvs", graphics::DataFormat::Float2});

  graphics::MeshDescriptor cube_desc{
      std::pair(cube_verts_normals_uvs, sizeof(cube_verts_normals_uvs)),
      std::pair(cube_indices_big, sizeof(cube_indices_big)), &cube_layout,
      "cobble_cube"};

  {
    u32 cobble_cube_mesh =
        scene_.AddMesh(cube_desc, glm::vec3{-7.5f, -9.0f, -7.5f});
    cube_desc.name = "paving_cube";
    u32 right_cube_mesh =
        scene_.AddMesh(cube_desc, glm::vec3{1.0f, 0.0f, 0.0f});
    cube_desc.name = "container_cube";
    u32 top_cube_mesh = scene_.AddMesh(cube_desc, glm::vec3{-1.0f, 0.0f, 0.0f});

    auto lit_paving_mat = asset_mgr_.LoadResource<graphics::Material>(
        "examples/resources/materials/lit.json");
    auto container_mat = asset_mgr_.LoadResource<graphics::Material>(
        "examples/resources/materials/lit.json");
    auto lit_cobble_mat = asset_mgr_.LoadResource<graphics::Material>(
        "examples/resources/materials/lit.json");

    lit_cobble_mat->SetTexture(
        asset_mgr_.LoadResource<graphics::Texture>(
            "examples/resources/textures/cobblestone_bc.png"),
        "TexDiffuse");
    lit_cobble_mat->SetTexture(
        asset_mgr_.LoadResource<graphics::Texture>(
            "examples/resources/textures/cobblestone_spec.png"),
        "TexSpecular");
    lit_paving_mat->SetTexture(asset_mgr_.LoadResource<graphics::Texture>(
                                   "examples/resources/textures/paving_bc.png"),
                               "TexDiffuse");
    lit_paving_mat->SetTexture(
        asset_mgr_.LoadResource<graphics::Texture>(
            "examples/resources/textures/paving_spec.png"),
        "TexSpecular");
    container_mat->SetTexture(asset_mgr_.LoadResource<graphics::Texture>(
                                  "examples/resources/textures/container.png"),
                              "TexDiffuse");
    container_mat->SetTexture(
        asset_mgr_.LoadResource<graphics::Texture>(
            "examples/resources/textures/container_spec.png"),
        "TexSpecular");

    scene_.AddMaterial(lit_cobble_mat, cobble_cube_mesh);
    scene_.AddMaterial(lit_paving_mat, right_cube_mesh);
    scene_.AddMaterial(container_mat, top_cube_mesh);

    // Turn cobble cube into big plane
    {
      auto *tr =
          scene_.SceneGraph().GetComponentPtr<graphics::TransformComponent>(
              cobble_cube_mesh);
      tr->scale = {15.0f, 1.0f, 15.0f};
      tr->dirty = true;
    }

    // make cubes rotate:
    {
      RotateComponent r{2.0f, 0.0f, 0.0f};
      scene_.SceneGraph().AddComponent(right_cube_mesh, r);
      r = {0.0f, 0.0f, 4.0f};
      scene_.SceneGraph().AddComponent(top_cube_mesh, r);
    }
    // {
    //   tinygltf::Model model;
    //   tinygltf::TinyGLTF loader;
    //   std::string err;
    //   std::string warn;
    //   bool ret = loader.LoadBinaryFromFile(
    //       &model, &err, &warn,
    //       "examples/resources/models/Box.glb"); // for binary glTF(.glb)
    //
    //   if (!warn.empty()) {
    //     HW_APP_WARN("WARNING FROM GLTF: {}", warn);
    //   }
    //
    //   if (!err.empty()) {
    //     HW_APP_ERROR("ERROR FROM GLTF: {}", err);
    //   }
    //
    //   if (!ret) {
    //     HW_APP_ERROR("Failed to parse GLTF");
    //   }
    //   HW_APP_INFO("GLTF parsing works :)")
    // }
  }
}

void Demo::OnRender(f64 delta) {
  scene_.Update(delta);
  renderer_.Clear(bgcolor_);
  scene_.SubmitAll();
}

void Demo::OnGuiRender(f64 delta) {
  (void)delta;
  EventDispatch(GuiRenderEvent{});
  scene_.OnGuiRender();
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

const char *Demo::GetProgramName() const { return kTitle.c_str(); }
