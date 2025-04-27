#include "Scene.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/GeometryComponent.hpp"
#include "heavy_weather/rendering/LightSourceComponent.hpp"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/RenderCommand.hpp"
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
    BufferDescriptor desc;
    {
      desc.size = 64 + 64 + 16;
      desc.count = 1;
      desc.type = BufferType::UniformBuffer;
    };
    matrices_ubo_ = renderer_.CreateBuffer(desc, nullptr);
    renderer_.Api().BindShaderResource(*matrices_ubo_, 0);
  }
  // lights ubo:
  {
    BufferDescriptor desc;
    {
      // One dirlight, kMaxPointLights pointlights
      desc.size =
          sizeof(DirLight) + (kMaxPointLights * sizeof(LightSourceComponent));
      desc.count = 1;
      desc.type = BufferType::UniformBuffer;
    };
    lights_ubo_ = renderer_.CreateBuffer(desc, nullptr);
    renderer_.Api().BindShaderResource(*lights_ubo_, 1);
  }

  dirlight_ = DirLight{
      {-0.2f, -1.0f, -0.3f, 0.0f},
      {0.1f, 0.1f, 0.1f, 0.0f},
      {0.4f, 0.4f, 0.4f, 0.0f},
      {0.0f, 0.0f, 0.0f, 0.0f},
  };

  HW_ASSERT(sizeof(LightSourceComponent) * kMaxPointLights ==
            lights_ubo_->Size() - sizeof(DirLight));

  // Initial write: dirlight + empty pointlights array
  renderer_.Api().WriteBufferData(*lights_ubo_, &dirlight_, 0,
                                  sizeof(DirLight));

  std::array<LightSourceComponent, kMaxPointLights> pointlights{};
  renderer_.Api().WriteBufferData(
      *lights_ubo_, pointlights.data(), sizeof(DirLight),
      sizeof(LightSourceComponent) * kMaxPointLights);
}

void Scene::Update(f64 delta) {
  camera_.ProcessInput(delta);
  camera_.Update();
  for (const auto &sys : systems_) {
    sys(scenegraph_, delta);
  }
  UpdateCameraMatrices();
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

void Scene::UpdateCameraMatrices() {
  struct Matrices {
    glm::mat4 proj;
    glm::mat4 view;
    glm::vec3 camera_world;
  };
  auto view = camera_.GetMatrix();
  auto proj = glm::perspective(glm::radians(camera_.Fov()),
                               float(renderer_.ViewPort().first) /
                                   float(renderer_.ViewPort().second),
                               camera_.Near(), camera_.Far());
  Matrices matrices{proj, view, camera_.Position()};
  renderer_.Api().WriteBufferData(*matrices_ubo_, &matrices, 0,
                                  (2 * sizeof(glm::mat4)) + 16);
}

// Query point lights, update their position and send them to shader along with
// hard-coded dir light
void Scene::ProcessLightSources() {
  auto lights = scenegraph_.Query<TransformComponent, LightSourceComponent>();
  HW_ASSERT(lights.size() <= kMaxPointLights);
  std::array<LightSourceComponent, kMaxPointLights> pointlights{};
  u64 i = 0;
  for (; i < lights.size(); ++i) {
    pointlights.at(i) =
        scenegraph_.GetComponent<LightSourceComponent>(lights[i]);
    auto tr =
        scenegraph_.GetComponent<TransformComponent>(lights[i]).translation;
    pointlights.at(i).position = glm::vec4(tr, 0.0f);
  }
  // fill rest of array with 0 so we don't get ghost lights after removing:
  memset(&pointlights.at(i), 0,
         (kMaxPointLights - i) * sizeof(LightSourceComponent));

  // TODO: Configurable DirLight
  // renderer_.Api().WriteBufferData(*lights_ubo_, &dirlight_, 0,
  //                                 sizeof(DirLight));
  renderer_.Api().WriteBufferData(
      *lights_ubo_, pointlights.data(), sizeof(DirLight),
      sizeof(LightSourceComponent) * kMaxPointLights);
}

void Scene::SubmitAll() {
  renderer_.ClearDepth();
  auto meshes =
      scenegraph_
          .Query<TransformComponent, GeometryComponent, MaterialComponent>();
  for (const auto &elem : meshes) {
    auto *transform = scenegraph_.GetComponentPtr<TransformComponent>(elem);
    auto *bufs = scenegraph_.GetComponentPtr<GeometryComponent>(elem);
    auto *mat = scenegraph_.GetComponentPtr<MaterialComponent>(elem);
    transform->ComputeMatrix();
    UniquePtr<RenderCommand> p =
        std::make_unique<rendercommands::RenderMeshCmd>(
            bufs, mat->material.get(), transform->matrix);
    renderer_.PushCommand(std::move(p));
  }
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
