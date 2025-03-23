#include "Renderer.hpp"

#include "heavy_weather/engine.h"
#include "heavy_weather/platform/Platform.hpp"
#include "heavy_weather/rendering/GeometryComponent.hpp"
#include "heavy_weather/rendering/ShaderProgram.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <utility>

namespace weather::graphics {

GeometryComponent Renderer::CreateBuffers(const MeshDescriptor &desc) {
  u32 *indices = desc.indices.first;
  u64 indices_sz = desc.indices.second;
  void *verts = desc.vertices.first;
  u64 vert_sz = desc.vertices.second;

  auto vert_count = desc.vertices.second / desc.layout->Stride();
  auto index_count = indices_sz / sizeof(u32);

  BufferDescriptor vdesc{vert_sz, vert_count, BufferType::VertexBuffer,
                         desc.layout};
  BufferDescriptor idesc{indices_sz, index_count, BufferType::IndexBuffer,
                         nullptr};

  return GeometryComponent{api_->CreateBuffer(vdesc, verts),
                           api_->CreateBuffer(idesc, indices)};
}

void Renderer::Submit(glm::mat4 &mvp, const Buffer &vbuf, const Buffer &ibuf,
                      Material &material) {
  auto shader = material.GetShader();
  UsePipeline(*shader);

  f32 time = PlatformGetTime();
  auto mvp_uniform = UniformDescriptor{"MVP", DataFormat::Mat4, &mvp};
  auto time_uniform =
      UniformDescriptor{"iGlobalTime", DataFormat::Float, &time};
  shader->BindUniform(time_uniform);
  shader->BindUniform(mvp_uniform);

  material.BindUniforms();

  api_->BindBuffer(vbuf);
  api_->BindBuffer(ibuf);
  api_->RenderIndexed(ibuf.GetCount());
}

SharedPtr<ShaderProgram> Renderer::CreatePipeline(ShaderDescriptor vsdesc,
                                                  ShaderDescriptor fsdesc) {
  UniquePtr<graphics::Shader> vs = api_->CreateShader(std::move(vsdesc));
  UniquePtr<graphics::Shader> fs = api_->CreateShader(std::move(fsdesc));
  graphics::PipelineDescriptor pdesc = {std::move(vs), std::move(fs)};
  return std::shared_ptr<ShaderProgram>(api_->CreatePipeline(pdesc));
}

void Renderer::UsePipeline(ShaderProgram &pipeline) {
  api_->UsePipeline(pipeline);
}

} // namespace weather::graphics
