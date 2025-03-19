#include "SceneManager.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/BuffersComponent.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"
#include "heavy_weather/rendering/Types.hpp"
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

void SceneManager::AddMesh(MeshDescriptor &desc) {
  // register mesh to scene
  u32 mesh = scene_.CreateEntity();
  scene_.AddComponent(mesh, renderer_.CreateBuffers(desc));
  scene_.AddComponent(mesh, MaterialComponent{});
  scene_.AddComponent(mesh, TransformComponent{});

// create and register a gui widget for the mesh
#ifdef HW_ENABLE_GUI
  scene_.AddComponent(mesh,
                      WidgetComponent{std::vector<WidgetFunc>{
                          TransformControl, MaterialPicker, DeleteButton}});
#endif
}

void SceneManager::SubmitAll() {
  auto view = camera_.GetMatrix();
  auto proj = glm::perspective(glm::radians(camera_.Fov()),
                               float(renderer_.ViewPort().first) /
                                   float(renderer_.ViewPort().second),
                               camera_.Near(), camera_.Far());

  auto meshes =
      scene_.Query<TransformComponent, BuffersComponent, MaterialComponent>();
  HW_ASSERT(scene_.Count() == meshes.size());
  for (const auto &elem : meshes) {
    auto &transform = scene_.GetComponent<TransformComponent>(elem);
    auto &bufs = scene_.GetComponent<BuffersComponent>(elem);
    auto &mat = scene_.GetComponent<MaterialComponent>(elem);
    // HW_ASSERT(mat->color == glm::vec4(0.1f, 0.3f, 0.7f, 1.0f));

    transform.ComputeMatrix();
    auto mvp = proj * view * transform.matrix;
    Buffer& vbuf = *bufs.vbuffer;
    Buffer& ibuf = *bufs.ibuffer;
    HW_ASSERT(vbuf.Type() == BufferType::VertexBuffer);
    HW_ASSERT(ibuf.Type() == BufferType::IndexBuffer);
    renderer_.Submit(mvp, vbuf, ibuf, &mat);
  }

  char title[12];
  auto widgets = scene_.Query<WidgetComponent>();
  for (const auto &e : widgets) {
    std::sprintf(title, "entity #%u", e);
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
