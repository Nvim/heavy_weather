/*
 * Bridge between SceneGraph and rest of Application.
 * Is owned by Application, owns SceneGraph, Is aware of Gui.
 * Encaplusates SceneGraph access.
 * */
#pragma once

#include "Scene.hpp"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <unordered_map>

namespace weather::graphics {

class SceneManager {
private:
  Scene scene_;
  Camera camera_;
  Gui &gui_;
  Renderer &renderer_;
  std::unordered_map<u64, u64> nodetogui_;

public:
  SceneManager(Renderer &renderer, Gui &gui, CameraParams &camera_params);
  void AddNode(MeshDescriptor &desc);
  void SubmitAll();
  void RemoveNode();

  // Delete all
  SceneManager(const SceneManager &) = delete;
  SceneManager(SceneManager &&) = delete;
  SceneManager &operator=(const SceneManager &) = delete;
  SceneManager &operator=(SceneManager &&) = delete;
  ~SceneManager() = default;
};

} // namespace weather::graphics
