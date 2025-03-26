#include "SceneManager.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/GeometryComponent.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "heavy_weather/scene/components/NameComponent.hpp"
#include "heavy_weather/scene/components/WidgetComponent.hpp"
#include <cstdio>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

namespace weather::graphics {

SceneManager::SceneManager(Renderer &renderer,
#ifdef HW_ENABLE_GUI
                           Gui &gui,
#endif
                           CameraParams &camera_params)
    : camera_{camera_params},
#ifdef HW_ENABLE_GUI
      gui_{gui},
#endif
      renderer_{renderer} {
  EventCallback<EntityRemovedEvent> evt =
      std::bind(&SceneManager::OnEntityRemoved, this, std::placeholders::_1);
  EventRegister(evt);
}

void SceneManager::Update(f64 delta) {
  camera_.ProcessInput(delta);
  camera_.Update();
}

u32 SceneManager::AddMesh(MeshDescriptor &desc, glm::vec3 coords, u32 entity) {
  // register mesh to scene
  u32 mesh{};
  if (entity == NEW_ENTITY) {
    mesh = scene_.CreateEntity();
  } else {
    mesh = entity;
  }
  scene_.AddComponent(mesh, renderer_.CreateGeometry(desc));
  // scene_.AddComponent(mesh, MaterialComponent{});
  auto tr = TransformComponent{};
  tr.translation = coords;
  tr.dirty = true;
  scene_.AddComponent(mesh, tr);
  if (desc.name) {
    scene_.AddComponent(mesh, NameComponent{desc.name});
  }

// create and register a gui widget for the mesh
#ifdef HW_ENABLE_GUI
  scene_.AddComponent(mesh,
                      WidgetComponent{std::vector<WidgetFunc>{
                          TransformControl, MaterialEditor, DeleteButton}});
#endif

  return mesh;
}

u32 SceneManager::AddMaterial(SharedPtr<Material> desc, u32 entity) {
  u32 e{};
  if (entity == NEW_ENTITY) {
    e = scene_.CreateEntity();
  } else {
    e = entity;
  }

  if (scene_.HasComponent<MaterialComponent>(e)) {
    auto m = scene_.GetComponent<MaterialComponent>(e);
    m.material = desc;
  } else {
    scene_.AddComponent(e, MaterialComponent{desc});
  }

  return e;
}

void SceneManager::SubmitAll() {
  renderer_.ClearDepth();
  auto view = camera_.GetMatrix();
  auto proj = glm::perspective(glm::radians(camera_.Fov()),
                               float(renderer_.ViewPort().first) /
                                   float(renderer_.ViewPort().second),
                               camera_.Near(), camera_.Far());

  auto meshes =
      scene_.Query<TransformComponent, GeometryComponent, MaterialComponent>();
  HW_ASSERT(scene_.Count() == meshes.size());
  for (const auto &elem : meshes) {
    auto &transform = scene_.GetComponent<TransformComponent>(elem);
    auto &bufs = scene_.GetComponent<GeometryComponent>(elem);
    auto &mat = scene_.GetComponent<MaterialComponent>(elem);
    HW_ASSERT(mat.material->GetShader() != nullptr);

    transform.ComputeMatrix();
    auto mvp = proj * view * transform.matrix;
    Buffer &vbuf = *bufs.vbuffer;
    Buffer &ibuf = *bufs.ibuffer;
    HW_ASSERT(vbuf.Type() == BufferType::VertexBuffer);
    HW_ASSERT(ibuf.Type() == BufferType::IndexBuffer);
    renderer_.Submit(mvp, vbuf, ibuf, *mat.material.get());
  }

  char title[32];
  auto widgets = scene_.Query<WidgetComponent>();
  for (const auto &e : widgets) {
    if (scene_.HasComponent<NameComponent>(e)) {
      std::snprintf(title, 31, "%s",
                    scene_.GetComponent<NameComponent>(e).name);
    } else {
      std::sprintf(title, "entity #%u", e);
    }
    if (Gui::BeginTreeNode(title)) {
      for (auto &fn : scene_.GetComponent<WidgetComponent>(e).funcs) {
        fn(gui_, scene_, e);
      }
      Gui::EndTreeNode();
    }
  }
  GarbageCollect();
}

void SceneManager::OnEntityRemoved(const EntityRemovedEvent &e) {
  HW_CORE_INFO("Adding entity {} to removals", e.GetID());
  removals_.push_back(e.GetID());
}

void SceneManager::GarbageCollect() {
  for (auto r : removals_) {
    HW_CORE_INFO("Removing entity {}", r);
    scene_.DestroyEntity(r);
  }
  removals_.clear();
}

} // namespace weather::graphics
