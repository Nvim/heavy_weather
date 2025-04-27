#include "Renderer.hpp"

#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/platform/Platform.hpp"
#include "heavy_weather/rendering/Backend/GL/GLAPI.hpp"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/GeometryComponent.hpp"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/RenderCommand.hpp"
#include "heavy_weather/rendering/ShaderProgram.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "heavy_weather/rendering/VertexLayout.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <memory>
#include <utility>

namespace weather::graphics {

static bool ValidateBufferDesc(const BufferDescriptor &desc) {
  if (desc.type == BufferType::Unknown) {
    return false;
  }
  if (desc.type == BufferType::UniformBuffer) {
    if (desc.size == 0) {
      return false;
    }
  }
  if (desc.type == BufferType::VertexBuffer) {
    if (desc.size == 0 || desc.count == 0 || desc.layout == nullptr) {
      return false;
    }
  }
  if (desc.type == BufferType::IndexBuffer) {
    if (desc.size == 0 || desc.count == 0) {
      return false;
    }
  }
  return true;
}

Renderer::Renderer(RendererInitParams &params) {
  HW_ASSERT_MSG(params.backend == Backend::OpenGL, "Only OpenGL is supported");
  api_ = std::unique_ptr<BackendAPI>(
      new GLBackendAPI(params.viewport.first, params.viewport.second,
                       params.depth_test, params.debug_mode));
};

void Renderer::PushCommand(UniquePtr<RenderCommand> command) {
  command_queue_.push(std::move(command));
}

bool Renderer::Begin() {
  if (recording_) {
    HW_CORE_ERROR("Can't begin recording commands: recording already started");
    return false;
  }
  if (!command_queue_.empty()) {
    HW_CORE_WARN("Renderer CommandQueue wasn't flushed before beginning frame");
    return false;
  }
  recording_ = true;
  return true;
}

bool Renderer::ProcessCommands() {
  if (!recording_) {
    HW_CORE_ERROR("Can't process commands: queue hasn't begun recording");
    return false;
  }
  while (!command_queue_.empty()) {
    auto cmd = std::move(command_queue_.front());
    if (!cmd->Execute(*api_)) {
      return false;
    }
    command_queue_.pop();
  }
  recording_ = false;
  return true;
}

UniquePtr<Buffer> Renderer::CreateBuffer(const BufferDescriptor &desc,
                                         void *data) {
  if (!ValidateBufferDesc(desc)) {
    HW_CORE_ERROR("Couldn't create buffer: invalid descriptor");
    HW_ASSERT(false);
    return nullptr;
  }
  return api_->CreateBuffer(desc, data);
}

GeometryComponent Renderer::CreateGeometry(const MeshDescriptor &desc) {
  u32 *indices = desc.indices.first;
  u64 indices_sz = desc.indices.second;
  void *verts = desc.vertices.first;
  u64 vert_sz = desc.vertices.second;

  auto vert_count = desc.vertices.second / desc.layout->Stride();
  auto index_count = indices_sz / sizeof(u32);

  BufferDescriptor vdesc;
  {
    vdesc.size = vert_sz;
    vdesc.count = vert_count;
    vdesc.type = BufferType::VertexBuffer;
    vdesc.layout = desc.layout;
  }
  BufferDescriptor idesc;
  {
    idesc.size = indices_sz;
    idesc.count = index_count;
    idesc.type = BufferType::IndexBuffer;
  }

  return GeometryComponent{api_->CreateBuffer(vdesc, verts),
                           api_->CreateBuffer(idesc, indices)};
}

SharedPtr<Texture> Renderer::CreateTexture(SharedPtr<Image> img) {
  return api_->CreateTexture(std::move(img));
}

SharedPtr<ShaderProgram> Renderer::CreatePipeline(SharedPtr<ShaderSource> vs,
                                                  SharedPtr<ShaderSource> fs) {
  UniquePtr<graphics::Shader> vert =
      api_->CreateShader(std::move(vs), ShaderType::VertexShader);
  UniquePtr<graphics::Shader> frag =
      api_->CreateShader(std::move(fs), ShaderType::FragmentShader);
  graphics::PipelineDescriptor pdesc = {std::move(vert), std::move(frag)};
  return std::shared_ptr<ShaderProgram>(api_->CreatePipeline(pdesc));
}

void Renderer::UsePipeline(ShaderProgram &pipeline) {
  api_->UsePipeline(pipeline);
}

} // namespace weather::graphics
