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
#include "heavy_weather/scene/systems/System.hpp"
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

  systems_.emplace_back(RotateSystem);

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
      // One dirlight, kMaxPointLights pointlights
      desc.size =
          sizeof(DirLight) + (kMaxPointLights * sizeof(LightSourceComponent));
      desc.count = 1;
      desc.binding = 1;
      desc.type = BufferType::UniformBuffer;
      desc.layout = &ubo_layout;
    };
    lights_ubo_ = renderer_.CreateBuffer(desc, nullptr);
  }

  dirlight_ = DirLight{
      {-0.2f, -1.0f, -0.3f, 0.0f},
      {0.1f, 0.1f, 0.1f, 0.0f},
      {0.4f, 0.4f, 0.4f, 0.0f},
      {0.0f, 0.0f, 0.0f, 0.0f},
  };
}

void Scene::Update(f64 delta) {
  camera_.ProcessInput(delta);
  camera_.Update();
  for (const auto &sys : systems_) {
    sys(scenegraph_, delta);
  }
  ProcessLightSources();
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
  scenegraph_.AddComponent(mesh, WidgetComponent{std::vector<WidgetFunc>{
                                     TransformControl, MaterialEditor,
                                     DeleteEntityButton, LightSourceControl}});
#endif

  return mesh;
}

u32 Scene::AddMesh(MeshDescriptor &desc, TransformComponent &transform,
                   u32 entity) {
  // register mesh to scene
  u32 mesh{};
  if (entity == NEW_ENTITY) {
    mesh = scenegraph_.CreateEntity();
  } else {
    mesh = entity;
  }
  scenegraph_.AddComponent(mesh, renderer_.CreateGeometry(desc));
  // scene_.AddComponent(mesh, MaterialComponent{});
  transform.dirty = true;
  scenegraph_.AddComponent(mesh, transform);
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

u32 Scene::AddLightSource(LightSourceComponent &comp, u32 entity) {
  scenegraph_.AddComponent(entity, comp);
  // create and register a gui widget for the mesh
#ifdef HW_ENABLE_GUI
  if (scenegraph_.HasComponent<WidgetComponent>(entity)) {
    auto *widgets = scenegraph_.GetComponentPtr<WidgetComponent>(entity);
    widgets->funcs.emplace_back(LightSourceControl);
  } else {
    scenegraph_.AddComponent(entity,
                             WidgetComponent{std::vector<WidgetFunc>{
                                 TransformControl, MaterialEditor,
                                 DeleteEntityButton, LightSourceControl}});
  }
#endif
  return entity;
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

// Query point lights, update their position and send them to shader along with
// hard-coded dir light
void Scene::ProcessLightSources() {
  auto lights = scenegraph_.Query<TransformComponent, LightSourceComponent>();
  HW_ASSERT(lights.size() <= kMaxPointLights);
  std::array<LightSourceComponent, kMaxPointLights> pointlights{};
  u16 i = 0;
  for (; i < lights.size(); ++i) {
    pointlights[i] = scenegraph_.GetComponent<LightSourceComponent>(lights[i]);
    auto tr =
        scenegraph_.GetComponent<TransformComponent>(lights[i]).translation;
    pointlights[i].position = glm::vec4(tr, 0.0f);
  }

  for (; i < kMaxPointLights; ++i) {
    pointlights[i] = LightSourceComponent{};
  }

  // Write pointlight & dirlight to ubo:
  struct PointAndDirLight {
    DirLight dirLight;
    std::array<LightSourceComponent, kMaxPointLights> pointlight;
  } s = {
      dirlight_,
      pointlights,
  };
  HW_ASSERT(sizeof(s) == lights_ubo_->Size());
  renderer_.WriteBufferData(*lights_ubo_, &s, lights_ubo_->Size());
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

  auto meshes =
      scenegraph_
          .Query<TransformComponent, GeometryComponent, MaterialComponent>();
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
  removals_.insert(e.GetID());
}

void Scene::GarbageCollect() {
  for (auto r : removals_) {
    HW_CORE_INFO("Removing entity {}", r);
    scenegraph_.DestroyEntity(r);
  }
  removals_.clear();
}

} // namespace weather::graphics
