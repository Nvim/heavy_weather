#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Mesh.hpp"
#include <vector>

namespace weather::graphics {

// TODO: not only meshes
// TODO: make actual use of id (can't remove for now)
class Scene {
public:
  u64 AddNode(UniquePtr<Mesh> node);
  std::vector<UniquePtr<Mesh>>::const_iterator GetBegin() const;
  std::vector<UniquePtr<Mesh>>::const_iterator GetEnd() const;
  // const std::vector<UniquePtr<Mesh>>& GetVector() const;

private:
  // Just a list of meshes for now
  std::vector<UniquePtr<Mesh>> scenegraph_;
};

} // namespace weather::graphics
