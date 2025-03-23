/*
 * Bridge between SceneGraph and rest of Application.
 * Is owned by Application, owns SceneGraph, Is aware of Gui.
 * Encaplusates SceneGraph access.
 * */
#pragma once

#include "heavy_weather/ecs/ecs.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/rendering/Camera.hpp"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <unordered_map>

#define NEW_ENTITY 0

namespace weather::graphics {

class SceneManager {
private:
  // Scene scene_;
  ECS scene_;
  Camera camera_;
#ifdef HW_ENABLE_GUI
  Gui &gui_;
#endif
  Renderer &renderer_;
  std::unordered_map<u64, u64> nodetogui_;
  // TODO: removals should prevent duplicates
  std::vector<u32> removals_; // Used to delay removals to the end of frame

public:
  SceneManager(Renderer &renderer,
#ifdef HW_ENABLE_GUI
               Gui &gui,
#endif
               CameraParams &camera_params);
  u32 AddMesh(MeshDescriptor &desc, u32 entity);
  u32 AddMaterial(SharedPtr<Material> desc, u32 entity);
  void Update(f64 delta);
  void SubmitAll();

  // Delete all
  SceneManager(const SceneManager &) = delete;
  SceneManager(SceneManager &&) = delete;
  SceneManager &operator=(const SceneManager &) = delete;
  SceneManager &operator=(SceneManager &&) = delete;
  ~SceneManager() = default;

private:
  void OnEntityRemoved(const EntityRemovedEvent &e);
  void GarbageCollect();
};

} // namespace weather::graphics
