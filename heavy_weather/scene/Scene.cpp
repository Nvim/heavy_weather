#include "Scene.hpp"

namespace weather::graphics {

static u64 s_id = 0;

u64 Scene::AddNode(UniquePtr<Mesh> node) {
  scenegraph_.push_back(std::move(node));
  return ++s_id;
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
