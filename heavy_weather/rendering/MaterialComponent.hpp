#pragma once

#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <glm/glm.hpp>

namespace weather::graphics {

struct MaterialComponent {
  SharedPtr<Material> material;
};

// struct MaterialComponent {
//   const char *name;
//   SharedPtr<Pipeline> shader;
//   std::vector<UniformDescriptor> uniforms;
//   // std::vector<SharedPtr<Texture>> textures; TODO
//
//   MaterialComponent(const char *name, SharedPtr<Pipeline> shader,
//                     std::vector<UniformDescriptor> uniforms)
//       : name(name), shader(std::move(shader)), uniforms(std::move(uniforms))
//       {}
//
//   MaterialComponent(const MaterialComponent &other) = default;
//   MaterialComponent(MaterialComponent &&other) noexcept
//       : name(other.name), shader(std::move(other.shader)),
//         uniforms(std::move(other.uniforms)) {}
//
//   MaterialComponent &operator=(const MaterialComponent &other) {
//     if (this != &other) {
//       this->name = other.name;
//       this->shader = other.shader;
//       this->uniforms = other.uniforms;
//     }
//     return *this;
//   }
//
//   MaterialComponent &operator=(MaterialComponent &&other) noexcept {
//     if (this != &other) {
//       this->name = other.name;
//       this->shader = std::move(other.shader);
//       this->uniforms = std::move(other.uniforms);
//     }
//     return *this;
//   }
// };

} // namespace weather::graphics
