#include "Scene.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include <algorithm>

namespace weather::graphics {

static u64 s_id = 0;

u64 Scene::AddNode(UniquePtr<Mesh> node) {
  node->SetID(++s_id);
  scenegraph_.push_back(std::move(node));
  return s_id;
}

void Scene::DeleteNode(u64 id) {
  auto elem = std::find_if(scenegraph_.begin(), scenegraph_.end(),
                           [&id](const UniquePtr<Mesh> &n) { return n->GetID() == id;});
  if (elem != scenegraph_.end()) {
    HW_CORE_DEBUG("Removing Scene Node #{}", id);
    scenegraph_.erase(elem);
  } else {
    HW_CORE_INFO("Couldn't remove Scene Node #{}", id);
  }
}

std::vector<UniquePtr<Mesh>>::const_iterator Scene::GetBegin() const {
  return scenegraph_.cbegin();
}

// const std::vector<UniquePtr<Mesh>>& Scene::GetVector() const {
//   return scenegraph_;
// }

std::vector<UniquePtr<Mesh>>::const_iterator Scene::GetEnd() const {
  return scenegraph_.cend();
}

} // namespace weather::graphics
