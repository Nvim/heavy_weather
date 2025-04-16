#include "Scene.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/GeometryComponent.hpp"
#include "heavy_weather/rendering/LightSourceComponent.hpp"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "heavy_weather/rendering/VertexLayout.hpp"
#include "heavy_weather/scene/components/NameComponent.hpp"
#include "heavy_weather/scene/components/WidgetComponent.hpp"
#include "imgui.h"
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

namespace weather::graphics {

Scene::Scene(Renderer &renderer, CameraParams &camera_params)
    : camera_{camera_params}, renderer_{renderer} {
  EventCallback<EntityRemovedEvent> evt =
      std::bind(&Scene::OnEntityRemoved, this, std::placeholders::_1);
  EventRegister(evt, this);

  // matrices ubo:
  {
    graphics::VertexLayout ubo_layout{};
    ubo_layout.AddAttribute({"projection", graphics::DataFormat::Mat4});
    ubo_layout.AddAttribute({"view", graphics::DataFormat::Mat4});
    BufferDescriptor desc;
    {
      desc.size = 64 + 64 + 16;
      desc.count = 1;
      desc.binding = 0;
      desc.type = BufferType::UniformBuffer;
      desc.layout = &ubo_layout;
    };
    matrices_ubo_ = renderer_.CreateBuffer(desc, nullptr);
  }
  // lights ubo:
  {
    graphics::VertexLayout ubo_layout{};
    ubo_layout.AddAttribute({"position", graphics::DataFormat::Float3});
    ubo_layout.AddAttribute({"ambient", graphics::DataFormat::Float3});
    ubo_layout.AddAttribute({"diffuse", graphics::DataFormat::Float3});
    ubo_layout.AddAttribute({"specular", graphics::DataFormat::Float3});
    BufferDescriptor desc;
    {
      desc.size = 4 * 16; // TODO: offset compute function
      desc.count = 1;
      desc.binding = 1;
      desc.type = BufferType::UniformBuffer;
      desc.layout = &ubo_layout;
    };
    lights_ubo_ = renderer_.CreateBuffer(desc, nullptr);
  }

  // Add a testing lightsource
  {
    auto l = scenegraph_.CreateEntity();
    LightSourceComponent c{
        {0.2f, 0.2f, 0.2f},
        {0.5f, 0.5f, 0.5f},
        {1.0f, 1.0f, 1.0f},
    };
    auto tr = TransformComponent{};
    tr.translation = {2.0f, 7.0f, 2.0f};
    tr.dirty = true;
    scenegraph_.AddComponent(l, c);
    scenegraph_.AddComponent(l, tr);
  }
}

// TODO: remove this wtf
struct TmpLight {
  glm::vec3 position;
  f32 pad1;
  glm::vec3 ambient;
  f32 pad2;
  glm::vec3 diffuse;
  f32 pad3;
  glm::vec3 specular;
  f32 pad4;
};

void Scene::Update(f64 delta) {
  camera_.ProcessInput(delta);
  camera_.Update();
}

u32 Scene::AddMesh(MeshDescriptor &desc, glm::vec3 coords, u32 entity) {
  // register mesh to scene
  u32 mesh{};
  if (entity == NEW_ENTITY) {
    mesh = scenegraph_.CreateEntity();
  } else {
    mesh = entity;
  }
  scenegraph_.AddComponent(mesh, renderer_.CreateGeometry(desc));
  // scene_.AddComponent(mesh, MaterialComponent{});
  auto tr = TransformComponent{};
  tr.translation = coords;
  tr.dirty = true;
  scenegraph_.AddComponent(mesh, tr);
  if (desc.name) {
    scenegraph_.AddComponent(mesh, NameComponent{desc.name});
  }

// create and register a gui widget for the mesh
#ifdef HW_ENABLE_GUI
  scenegraph_.AddComponent(
      mesh, WidgetComponent{std::vector<WidgetFunc>{
                TransformControl, MaterialEditor, DeleteEntityButton}});
#endif

  return mesh;
}

u32 Scene::AddMaterial(SharedPtr<Material> desc, u32 entity) {
  u32 e{};
  if (entity == NEW_ENTITY) {
    e = scenegraph_.CreateEntity();
  } else {
    e = entity;
  }

  if (scenegraph_.HasComponent<MaterialComponent>(e)) {
    auto m = scenegraph_.GetComponent<MaterialComponent>(e);
    m.material = desc;
  } else {
    scenegraph_.AddComponent(e, MaterialComponent{desc});
  }

  return e;
}

// TODO: delete this again
struct Matrices {
  glm::mat4 proj;
  glm::mat4 view;
  glm::vec3 camera_world;
};
void Scene::SubmitAll() {
  renderer_.ClearDepth();
  auto view = camera_.GetMatrix();
  auto proj = glm::perspective(glm::radians(camera_.Fov()),
                               float(renderer_.ViewPort().first) /
                                   float(renderer_.ViewPort().second),
                               camera_.Near(), camera_.Far());
  Matrices matrices{proj, view, camera_.Position()};
  renderer_.WriteBufferData(*matrices_ubo_, &matrices,
                            (2 * sizeof(glm::mat4)) + 16);

  {
    auto lights = scenegraph_.Query<TransformComponent, LightSourceComponent>();
    HW_ASSERT(lights.size() == 1);
    auto component = scenegraph_.GetComponent<LightSourceComponent>(lights[0]);
    TmpLight l{
        // glm::vec4(
        scenegraph_.GetComponent<TransformComponent>(lights[0]).translation,
        0.0f,
        // 0.0f),
        // glm::vec4(
        component.ambient, 0.0f,
        // 0.0f),
        // glm::vec4(
        component.diffuse, 0.0f,
        // 0.0f),
        // glm::vec4(
        component.specular, 0.0f,
        // 0.0f),
    };
    renderer_.WriteBufferData(*lights_ubo_, &l, lights_ubo_->Size());
  }

  auto meshes =
      scenegraph_
          .Query<TransformComponent, GeometryComponent, MaterialComponent>();
  HW_ASSERT(scenegraph_.Count() == meshes.size() + 1);
  for (const auto &elem : meshes) {
    auto &transform = scenegraph_.GetComponent<TransformComponent>(elem);
    auto &bufs = scenegraph_.GetComponent<GeometryComponent>(elem);
    auto &mat = scenegraph_.GetComponent<MaterialComponent>(elem);
    HW_ASSERT(mat.material->GetShader() != nullptr);

    transform.ComputeMatrix();
    auto mvp = proj * view * transform.matrix;
    Buffer &vbuf = *bufs.vbuffer;
    Buffer &ibuf = *bufs.ibuffer;
    HW_ASSERT(vbuf.Type() == BufferType::VertexBuffer);
    HW_ASSERT(ibuf.Type() == BufferType::IndexBuffer);
    renderer_.Submit(mvp, transform.matrix, vbuf, ibuf, *mat.material.get());
  }
  GarbageCollect();
}

void Scene::OnGuiRender() {
#ifdef HW_ENABLE_GUI
  ImGui::Begin("Scene");
  char title[32];
  auto widgets = scenegraph_.Query<WidgetComponent>();
  for (const auto &e : widgets) {
    if (scenegraph_.HasComponent<NameComponent>(e)) {
      std::snprintf(title, 31, "%s",
                    scenegraph_.GetComponent<NameComponent>(e).name);
    } else {
      std::sprintf(title, "entity #%u", e);
    }
    if (Gui::BeginTreeNode(title)) {
      for (auto &fn : scenegraph_.GetComponent<WidgetComponent>(e).funcs) {
        fn(scenegraph_, e);
      }
      Gui::EndTreeNode();
    }
  }
  ImGui::End();
#endif
}

void Scene::OnEntityRemoved(const EntityRemovedEvent &e) {
  HW_CORE_INFO("Adding entity {} to removals", e.GetID());
  removals_.push_back(e.GetID());
}

void Scene::GarbageCollect() {
  for (auto r : removals_) {
    HW_CORE_INFO("Removing entity {}", r);
    scenegraph_.DestroyEntity(r);
  }
  removals_.clear();
}

} // namespace weather::graphics
