#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/BackendApi.hpp"
#include <glm/glm.hpp>

namespace weather {
class Image;
class ShaderSource;

namespace graphics {

class Texture;
class Material;
struct GeometryComponent;

class Renderer {
public:
  explicit Renderer(RendererInitParams &params);

  void Clear(glm::vec4 col) { api_->Clear(col); }
  void ClearDepth() { api_->ClearDepthBuffer(); }

  void Submit(glm::mat4 &mvp, glm::mat4 &model, const Buffer &vbuf,
              const Buffer &ibuf, Material &material);

  GeometryComponent CreateGeometry(const MeshDescriptor &desc);
  UniquePtr<Buffer> CreateBuffer(const BufferDescriptor &desc, void *data);
  SharedPtr<Texture> CreateTexture(SharedPtr<Image> img);
  SharedPtr<ShaderProgram> CreatePipeline(SharedPtr<ShaderSource> vs,
                                          SharedPtr<ShaderSource> fs);

  void UsePipeline(ShaderProgram &pipeline);
  void WriteBufferData(const Buffer &buf, void *data, u64 data_sz);
  void Resize(std::pair<u16, u16> new_sz) { api_->Resize(new_sz); }
  std::pair<u16, u16> ViewPort() const { return api_->ViewPort(); }

private:
  UniquePtr<BackendAPI> api_ = nullptr;
};

} // namespace graphics
} // namespace weather
