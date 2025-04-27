#include "RenderCommand.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/rendering/BackendApi.hpp"
#include "heavy_weather/rendering/Buffer.hpp"
#include <heavy_weather/rendering/GeometryComponent.hpp>
#include <heavy_weather/rendering/Material.hpp>

namespace weather::graphics::rendercommands {

RenderMeshCmd::RenderMeshCmd(GeometryComponent *geometry, Material *material,
                             glm::mat4 model)
    : model_{model}, geometry_{geometry}, material_{material} {}

bool RenderMeshCmd::Execute(BackendAPI &api) {
  if (material_ == nullptr || geometry_ == nullptr) {
    HW_CORE_CRITICAL("RenderMeshCmd failed: invalid material or geometry");
    return false;
  }

  Buffer &vbuf = *geometry_->vbuffer;
  Buffer &ibuf = *geometry_->ibuffer;
  if ((vbuf.Type() != BufferType::VertexBuffer) ||
      (ibuf.Type() != BufferType::IndexBuffer)) {
    HW_CORE_CRITICAL("RenderMeshCmd failed: invalid index/vertex buffer type");
    return false;
  }

  auto &shader = *material_->GetShader();
  api.UsePipeline(shader);
  auto model_uniform = UniformDescriptor{"Model", DataFormat::Mat4, &model_};
  shader.BindUniform(model_uniform);
  material_->BindUniforms();

  api.SetVertexBuffer(vbuf);
  api.SetIndexBuffer(ibuf);
  api.RenderIndexed(ibuf.Count());
  return true;
}
} // namespace weather::graphics::rendercommands
