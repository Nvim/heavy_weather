#include "Renderer.hpp"

#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/platform/Platform.hpp"
#include "heavy_weather/rendering/Backend/GL/GLAPI.hpp"
#include "heavy_weather/rendering/Buffer.hpp"
#include "heavy_weather/rendering/GeometryComponent.hpp"
#include "heavy_weather/rendering/Material.hpp"
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

void Renderer::Submit(glm::mat4 &mvp, glm::mat4 &model, const Buffer &vbuf,
                      const Buffer &ibuf, Material &material) {
  auto shader = material.GetShader();
  UsePipeline(*shader);

  f32 time = PlatformGetTime(); // TODO: get delta!
  auto mvp_uniform = UniformDescriptor{"MVP", DataFormat::Mat4, &mvp};
  auto model_uniform = UniformDescriptor{"Model", DataFormat::Mat4, &model};
  auto time_uniform =
      UniformDescriptor{"uGlobalTime", DataFormat::Float, &time};
  shader->BindUniform(time_uniform);
  shader->BindUniform(mvp_uniform);
  shader->BindUniform(model_uniform);

  material.BindUniforms();

  api_->SetVertexBuffer(vbuf);
  api_->SetIndexBuffer(ibuf);
  api_->RenderIndexed(ibuf.Count());
  // api_->Render();
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
