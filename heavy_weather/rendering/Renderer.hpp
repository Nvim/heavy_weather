#pragma once

#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Backend/GL/GLAPI.hpp"
#include "heavy_weather/rendering/BackendApi.hpp"
#include "heavy_weather/rendering/BuffersComponent.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <glm/glm.hpp>

namespace weather::graphics {

class Renderer {
public:
  Renderer(RendererInitParams &params) {
    HW_ASSERT_MSG(params.backend == Backend::OpenGL,
                  "Only OpenGL is supported");
    api_ = std::unique_ptr<BackendAPI>(
        new GLBackendAPI(params.viewport.first, params.viewport.second,
                         params.depth_test, params.debug_mode));
  };

  BuffersComponent CreateBuffers(const MeshDescriptor &desc);
  // UniquePtr<Mesh> CreateMesh(UniquePtr<Buffer> v, UniquePtr<Buffer> i);
  void Clear(glm::vec4 col) { api_->Clear(col); }
  void ClearDepth() { api_->ClearDepthBuffer(); }
  // void Submit(Mesh &mesh, glm::mat4 &mvp);
  void Submit(glm::mat4 &mvp, const Buffer &vbuf, const Buffer &ibuf,
              Material &material);
  SharedPtr<ShaderProgram> CreatePipeline(ShaderDescriptor vsdesc,
                                          ShaderDescriptor fsdesc);
  void UsePipeline(ShaderProgram &pipeline);
  void Resize(std::pair<u16, u16> new_sz) { api_->Resize(new_sz); }

  std::pair<u16, u16> ViewPort() const { return api_->ViewPort(); }

private:
  UniquePtr<BackendAPI> api_ = nullptr;
};

} // namespace weather::graphics
