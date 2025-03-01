#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Mesh.hpp"
#include <vector>

namespace weather::graphics {

// TODO: not only meshes
class Scene {
public:
  u64 AddNode(UniquePtr<Mesh> node);
  void DeleteNode(u64 id);
  std::vector<UniquePtr<Mesh>>::const_iterator GetBegin() const;
  std::vector<UniquePtr<Mesh>>::const_iterator GetEnd() const;
  void GarbageCollect();
  // const std::vector<UniquePtr<Mesh>>& GetVector() const;

private:
  // Just a list of meshes for now
  std::vector<UniquePtr<Mesh>> scenegraph_;

  // Same as in Gui.hpp
  std::vector<u64> removals_;
  
};

} // namespace weather::graphics
