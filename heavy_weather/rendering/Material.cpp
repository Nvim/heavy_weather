#include "Material.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

void Material::SetShader(const SharedPtr<ShaderProgram> &shader) {
  HW_ASSERT_MSG(shader.get() != nullptr, "can't use null shader");
  // TODO: add a way to check that shader was compiled successfully
  shader_ = shader;
}

#define BIND_LOOP(map)                                                         \
  for (auto it : (map)) {                                                      \
    d.name = it.first.c_str();                                                 \
    d.data = &it.second;                                                       \
    shader_->BindUniform(d);                                                   \
  }

void Material::BindUniforms() {
  // if (!dirty_) {
  //   return;
  // }
  UniformDescriptor d{};
  // clang-format off
  d.format = DataFormat::Int;
  for (auto& tex : textures_) {
    d.name = tex.first.c_str();
    d.data = tex.second->UnitPtr();
    shader_->BindUniform(d);
  }
  BIND_LOOP(ints_)
  d.format = DataFormat::Float; BIND_LOOP(floats_)
  d.format = DataFormat::Float2; BIND_LOOP(float2s_)
  d.format = DataFormat::Float3; BIND_LOOP(float3s_)
  d.format = DataFormat::Float4; BIND_LOOP(float4s_)
  d.format = DataFormat::Mat3; BIND_LOOP(mat3s_)
  d.format = DataFormat::Mat4; BIND_LOOP(mat4s_)
  // clang-format on
  // dirty_ = false;
}
#undef BIND_LOOP

} // namespace weather::graphics
