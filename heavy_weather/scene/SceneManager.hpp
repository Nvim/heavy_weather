/*
 * Bridge between SceneGraph and rest of Application.
 * Is owned by Application, owns SceneGraph, Is aware of Gui.
 * Encaplusates SceneGraph access.
 * */
#pragma once

#include "heavy_weather/ecs/ecs.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/Camera.hpp"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include <glm/fwd.hpp>

#define NEW_ENTITY 0

namespace weather::graphics {

class Texture;
class Renderer;
class Material;

class SceneManager {
private:
  // Scene scene_;
  ECS scene_;
  Camera camera_;
#ifdef HW_ENABLE_GUI
  Gui &gui_;
#endif
  Renderer &renderer_;
  // TODO: removals should prevent duplicates
  std::vector<u32> removals_; // Used to delay removals to the end of frame

public:
  SceneManager(Renderer &renderer,
#ifdef HW_ENABLE_GUI
               Gui &gui,
#endif
               CameraParams &camera_params);
  u32 AddMesh(MeshDescriptor &desc, glm::vec3 coords = {0.0f, 0.0f, 0.0f},
              u32 entity = NEW_ENTITY);
  u32 AddMaterial(SharedPtr<Material> desc, u32 entity);
  u32 AddTexture(SharedPtr<Texture> tex, u32 entity);
  void Update(f64 delta);
  void SubmitAll();
  void OnGuiRender();

  // Delete all
  SceneManager(const SceneManager &) = delete;
  SceneManager(SceneManager &&) = delete;
  SceneManager &operator=(const SceneManager &) = delete;
  SceneManager &operator=(SceneManager &&) = delete;
  ~SceneManager() = default;

private:
  void OnEntityRemoved(const EntityRemovedEvent &e);
  void GarbageCollect();
  UniquePtr<Buffer> matrices_ubo_{nullptr};
};

} // namespace weather::graphics
