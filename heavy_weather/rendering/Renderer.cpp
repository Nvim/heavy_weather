#include "Renderer.hpp"

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Pipeline.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <utility>

namespace weather::graphics {

UniquePtr<Mesh> Renderer::CreateMesh(const MeshDescriptor &desc) {
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

  auto vbuf = api_->CreateBuffer(vdesc, verts);
  auto ibuf = api_->CreateBuffer(idesc, indices);
  return std::make_unique<Mesh>(std::move(vbuf), std::move(ibuf));
}

UniquePtr<Mesh> Renderer::CreateMesh(UniquePtr<Buffer> v, UniquePtr<Buffer> i) {
  return std::make_unique<Mesh>(std::move(v), std::move(i));
}

void Renderer::Submit(Mesh &mesh) {
  api_->ClearDepthBuffer();
  mesh.Transform()->ComputeMatrix();
  glm::mat4 model = mesh.Transform()->GetMatrix();
  auto view =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                          float(api_->ViewPort().first) /
                                              float(api_->ViewPort().second),
                                          0.1f, 100.0f);
  auto mvp = projection * view * model;
  auto uniform_desc = UniformDescriptor{"MVP", DataFormat::Mat4, &mvp};
  api_->BindUniform(uniform_desc);
  api_->BindUniform(mesh.Material().uniform);
  api_->BindBuffer(mesh.VertexBuffer());
  api_->BindBuffer(mesh.IndexBuffer());
  api_->RenderIndexed(mesh.IndexBuffer().GetCount());
}

UniquePtr<Pipeline> Renderer::CreatePipeline(ShaderDescriptor vsdesc,
                                             ShaderDescriptor fsdesc) {
  UniquePtr<graphics::Shader> vs = api_->CreateShader(std::move(vsdesc));
  UniquePtr<graphics::Shader> fs = api_->CreateShader(std::move(fsdesc));
  graphics::PipelineDescriptor pdesc = {std::move(vs), std::move(fs)};
  return std::unique_ptr<Pipeline>(api_->CreatePipeline(pdesc));
}

void Renderer::UsePipeline(Pipeline &pipeline) { api_->UsePipeline(pipeline); }

} // namespace weather::graphics
