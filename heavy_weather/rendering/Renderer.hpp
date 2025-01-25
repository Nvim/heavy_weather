#pragma once

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Backend/GL/GLAPI.hpp"
#include "heavy_weather/rendering/BackendApi.hpp"
#include "heavy_weather/rendering/Mesh.hpp"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

class Renderer {
public:
  Renderer(Backend backend) {
    HW_ASSERT(backend == Backend::OpenGL);
    api_ = std::unique_ptr<BackendAPI>(new GLBackendAPI);
  };

  UniquePtr<Mesh> CreateMesh(const MeshDescriptor &desc);
  UniquePtr<Mesh> CreateMesh(UniquePtr<Buffer> v, UniquePtr<Buffer> i);
  void Clear() { api_->Clear(); }
  void Submit(Mesh &mesh);
  UniquePtr<Pipeline> CreatePipeline(ShaderDescriptor vsdesc,
                                     ShaderDescriptor fsdesc);
  void UsePipeline(Pipeline &pipeline);

private:
  UniquePtr<BackendAPI> api_ = nullptr;
};

} // namespace weather::graphics
