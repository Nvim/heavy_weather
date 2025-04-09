#include "Material.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/ShaderProgram.hpp"
#include "heavy_weather/rendering/Texture.hpp"

namespace weather::graphics {
Material::Material(SharedPtr<ShaderProgram> shader)
    : name_{shader->Name()}, shader_{std::move(shader)} {}

Material::Material(SharedPtr<ShaderProgram> shader, std::string &&name)
    : name_{std::move(name)}, shader_{std::move(shader)} {}

Material::Material(SharedPtr<ShaderProgram> shader, std::string &&name,
                   const std::filesystem::path &path)
    : name_{std::move(name)}, path_{path}, shader_{std::move(shader)} {}

Material::Material(const Material &other)
    : path_(other.path_), shader_(other.shader_), ints_(other.ints_),
      floats_(other.floats_), float2s_(other.float2s_),
      float3s_(other.float3s_), float4s_(other.float4s_), mat3s_(other.mat3s_),
      mat4s_(other.mat4s_), textures_(other.textures_) {
  // no need to find, other exists
  name_ = fmt::format("{}#{}", other.name_,
                      Material::instance_count[other.name_]++);
}
Material::Material(Material &&other) noexcept
    : path_(std::move(other.path_)), shader_(std::move(other.shader_)),
      ints_(std::move(other.ints_)), floats_(std::move(other.floats_)),
      float2s_(std::move(other.float2s_)), float3s_(std::move(other.float3s_)),
      float4s_(std::move(other.float4s_)), mat3s_(std::move(other.mat3s_)),
      mat4s_(std::move(other.mat4s_)), textures_(std::move(other.textures_)) {
  name_ = fmt::format("{}#{}", other.name_,
                      Material::instance_count[other.name_] + 1);
  Material::instance_count[name_]--;
}
Material &Material::operator=(const Material &other) {
  if (&other != this) {
    name_ = fmt::format("{}#{}", other.name_,
                        Material::instance_count[other.name_]++);
    path_ = other.path_;
    shader_ = other.shader_;
    ints_ = other.ints_;
    floats_ = other.floats_;
    float2s_ = other.float2s_;
    float3s_ = other.float3s_;
    float4s_ = other.float4s_;
    mat3s_ = other.mat3s_;
    mat4s_ = other.mat4s_;
    textures_ = other.textures_;
  }
  return *this;
}

Material &Material::operator=(Material &&other) noexcept {
  if (&other != this) {
    name_ = fmt::format("{}#{}", other.name_,
                        Material::instance_count[other.name_]++);
    path_ = std::move(other.path_);
    shader_ = std::move(other.shader_);
    ints_ = std::move(other.ints_);
    floats_ = std::move(other.floats_);
    float2s_ = std::move(other.float2s_);
    float3s_ = std::move(other.float3s_);
    float4s_ = std::move(other.float4s_);
    mat3s_ = std::move(other.mat3s_);
    mat4s_ = std::move(other.mat4s_);
    textures_ = std::move(other.textures_);
    Material::instance_count[name_]--;
  }
  return *this;
}

i64 Material::GetInstanceCount() const { return instance_count[name_]; }
SharedPtr<ShaderProgram> Material::GetShader() { return shader_; }
void Material::SetPath(const std::filesystem::path &path) { path_ = path; }
const std::unordered_map<std::string, SharedPtr<Texture>> &
Material::GetTextures() const {
  return textures_;
}
const std::string &Material::Name() const { return name_; }
const std::filesystem::path &Material::Path() const { return path_; }

void Material::SetShader(const SharedPtr<ShaderProgram> &shader) {
  HW_ASSERT_MSG(shader.get() != nullptr, "can't use null shader");
  // TODO: add a way to check that shader was compiled successfully
  shader_ = shader;
}

void Material::SetTexture(const SharedPtr<Texture> &tex,
                          const std::string &uniform) {
  // const auto& name = tex->Name();
  auto it = textures_.find(uniform);
  if (it == textures_.end()) {
    HW_CORE_WARN("Material doesn't accept textures for uniform name {}.");
  }
  textures_[uniform] = tex;
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
