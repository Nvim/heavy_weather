/* *
 * Renderer turns out to be mostly a wrapper around an API. Since BackendAPI is
 * already an API-agnostic interface, user code could be good using it directly
 * instead.
 * Starting from 3dd1c69577ec, I'll reduce it's use in new code and try to keep
 * it as a container/owner for the actual api instance.
 * Using pimpl pattern over interface may be interesting later too, idk.
 * */
#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/BackendApi.hpp"
#include "heavy_weather/rendering/RenderCommand.hpp"
#include <glm/glm.hpp>
#include <queue>

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

  bool Begin();
  void PushCommand(UniquePtr<RenderCommand> command);
  bool ProcessCommands();

  BackendAPI &Api() const { return *api_; }
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
  std::queue<UniquePtr<RenderCommand>> command_queue_;
  bool recording_{false};
};

} // namespace graphics
} // namespace weather
