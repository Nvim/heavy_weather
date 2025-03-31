#pragma once

#include "glm/glm.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/ShaderProgram.hpp"
#include "heavy_weather/rendering/Texture.hpp"
#include <unordered_map>
#include <utility>
namespace weather::graphics {

class Material {

private:
  std::string name_;
  SharedPtr<ShaderProgram> shader_;
  static inline std::unordered_map<std::string, i32>
      instance_count; // instance count per name
  std::unordered_map<std::string, i32> ints_;
  std::unordered_map<std::string, f32> floats_;
  std::unordered_map<std::string, glm::vec2> float2s_;
  std::unordered_map<std::string, glm::vec3> float3s_;
  std::unordered_map<std::string, glm::vec4> float4s_;
  std::unordered_map<std::string, glm::mat3> mat3s_;
  std::unordered_map<std::string, glm::mat4> mat4s_;
  std::unordered_map<std::string, SharedPtr<Texture>> textures_;

public:
  Material(SharedPtr<ShaderProgram> shader)
      : name_{shader->Name()}, shader_{std::move(shader)} {}

  Material(SharedPtr<ShaderProgram> shader, std::string &&name)
      : name_{std::move(name)}, shader_{std::move(shader)} {}

  Material(const Material &);
  Material(Material &&) noexcept;
  Material &operator=(const Material &);
  Material &operator=(Material &&) noexcept;

  SharedPtr<ShaderProgram> GetShader() { return shader_; }
  void SetShader(const SharedPtr<ShaderProgram> &shader);
  const std::unordered_map<std::string, SharedPtr<Texture>> &
  GetTextures() const {
    return textures_;
  }

  i64 GetInstanceCount() const;

  void BindUniforms();
  const std::string &Name() const { return name_; }

  template <typename T>
  void SetUniformValue(std::string &&name, const T &value) {
    // clang-format off
    if constexpr      (std::is_same_v<T, i32>)       { ints_[std::move(name)] =    value; }
    else if constexpr (std::is_same_v<T, f32>)       { floats_[std::move(name)] =  value; } 
    else if constexpr (std::is_same_v<T, glm::vec2>) { float2s_[std::move(name)] = value; } 
    else if constexpr (std::is_same_v<T, glm::vec3>) { float3s_[std::move(name)] = value; } 
    else if constexpr (std::is_same_v<T, glm::vec4>) { float4s_[std::move(name)] = value; } 
    else if constexpr (std::is_same_v<T, glm::mat3>) { mat3s_[std::move(name)] =   value; } 
    else if constexpr (std::is_same_v<T, glm::mat4>) { mat4s_[std::move(name)] =   value; } 
    else if constexpr (std::is_same_v<T, SharedPtr<Texture>>) { textures_[std::move(name)] =   value; } 
    else { HW_CORE_ERROR("Couldn't set uniform value, bad type") }
    // clang-format on
  }

#define GET_VALUE(map, default)                                                \
  auto it = (map).find(name);                                                  \
  if (it != (map).end()) {                                                     \
    return it->second;                                                         \
  }                                                                            \
  HW_CORE_WARN("Couldn't get uniform value, name is not present")              \
  return (default);

  template <typename T> const T &GetUniformValue(std::string &name) {
    // clang-format off
    if constexpr      (std::is_same_v<T, i32>)       { GET_VALUE(ints_,   0); }
    else if constexpr (std::is_same_v<T, f32>)       { GET_VALUE(floats_, 0.f); }
    else if constexpr (std::is_same_v<T, glm::vec2>) { GET_VALUE(float2s_, glm::vec2{0.f}); }
    else if constexpr (std::is_same_v<T, glm::vec3>) { GET_VALUE(float3s_, glm::vec3{0.f}); }
    else if constexpr (std::is_same_v<T, glm::vec4>) { GET_VALUE(float4s_, glm::vec4{0.f}); }
    else if constexpr (std::is_same_v<T, glm::mat3>) { GET_VALUE(mat3s_, glm::mat3{}); }
    else if constexpr (std::is_same_v<T, glm::mat4>) { GET_VALUE(mat4s_, glm::mat4{}); }
    else if constexpr (std::is_same_v<T, SharedPtr<Texture>>) { GET_VALUE(textures_, SharedPtr<Texture>{nullptr}); } 
    else {
      HW_ASSERT_MSG(false, "Couldn't get uniform value, bad type");
    }
    // clang-format on
  }
#undef GET_VALUE

  template <typename T> auto GetRange() {
    // clang-format off
    if constexpr      (std::is_same_v<T, i32>)       { return std::pair(ints_.begin(), ints_.end()); }
    else if constexpr (std::is_same_v<T, f32>)       { return std::pair(floats_.begin(), floats_.end()); }
    else if constexpr (std::is_same_v<T, glm::vec2>) { return std::pair(float2s_.begin(), float2s_.end()); }
    else if constexpr (std::is_same_v<T, glm::vec3>) { return std::pair(float3s_.begin(), float3s_.end()); }
    else if constexpr (std::is_same_v<T, glm::vec4>) { return std::pair(float4s_.begin(), float4s_.end()); }
    else if constexpr (std::is_same_v<T, glm::mat3>) { return std::pair(mat3s_.begin(), mat3s_.end()); }
    else if constexpr (std::is_same_v<T, glm::mat4>) { return std::pair(mat4s_.begin(), mat4s_.end()); }
    else if constexpr (std::is_same_v<T, SharedPtr<Texture>>) { return std::pair(textures_.begin(), textures_.end()); }
    else {
      HW_ASSERT_MSG(false, "Couldn't get uniform value, bad type");
    }
    // clang-format on
  }
};

} // namespace weather::graphics
