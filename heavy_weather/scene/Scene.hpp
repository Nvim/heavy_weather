/*
 * Bridge between SceneGraph and rest of Application.
 * Is owned by Application, owns SceneGraph, although it has public access.
 * */
#pragma once

#include "heavy_weather/ecs/ecs.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/Camera.hpp"
#include "heavy_weather/scene/systems/System.hpp"
#include <glm/fwd.hpp>
#include <unordered_set>

#define NEW_ENTITY 0

namespace weather::graphics {

struct DirLight {
  glm::vec4 direction;
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
};

class Texture;
class Renderer;
class Material;
struct TransformComponent;
struct LightSourceComponent;

using std::unordered_set;
using std::vector;

class Scene {
private:
  ECS scenegraph_;
  Camera camera_;
  Renderer &renderer_;
  vector<System> systems_;
  unordered_set<u32> removals_; // Used to delay removals to the end of frame
  DirLight dirlight_;
  static constexpr u16 kMaxPointLights = 5;

public:
  Scene(Renderer &renderer, CameraParams &camera_params);

  ECS &SceneGraph() { return scenegraph_; }
  void Update(f64 delta);
  void SubmitAll();
  void OnGuiRender();
  void GarbageCollect();

  // Helpers:
  u32 AddMesh(MeshDescriptor &desc, glm::vec3 coords = {0.0f, 0.0f, 0.0f},
              u32 entity = NEW_ENTITY);
  u32 AddMesh(MeshDescriptor &desc, TransformComponent &transform,
              u32 entity = NEW_ENTITY);
  u32 AddLightSource(LightSourceComponent &comp, u32 entity);
  u32 AddMaterial(SharedPtr<Material> desc, u32 entity);
  u32 AddTexture(SharedPtr<Texture> tex, u32 entity);

  // Delete all
  Scene(const Scene &) = delete;
  Scene(Scene &&) = delete;
  Scene &operator=(const Scene &) = delete;
  Scene &operator=(Scene &&) = delete;
  ~Scene() = default;

private:
  void UpdateCameraMatrices();
  void ProcessLightSources();
  void OnEntityRemoved(const EntityRemovedEvent &e);
  UniquePtr<Buffer> matrices_ubo_{nullptr};
  UniquePtr<Buffer> lights_ubo_{nullptr};
};

} // namespace weather::graphics
