#pragma once

/*
 * AssetManager owns an AssetLibrary for every asset type.
 * Unline AssetLibrary, it has acces to a renderer, so that it can instanciate
 * GPU resources such as Textures or Shader pipelines.
 * The primitive Image/ShaderSource objects indirectly created are added to
 * their respective AssetLibrary.
 * */

#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Texture.hpp"
#include "heavy_weather/resources/AssetLibrary.hpp"
#include "heavy_weather/resources/Image.hpp"
#include "heavy_weather/resources/ShaderSource.hpp"
#include <filesystem>

namespace weather {

class AssetManager {
  using Texture = graphics::Texture;
  using Material = graphics::Material;
  using ShaderProgram = graphics::ShaderProgram;
  using Renderer = graphics::Renderer;

public:
  AssetManager(Renderer *renderer) : renderer_{renderer} {}

  template <typename T>
  SharedPtr<T> LoadResource(const std::filesystem::path &path) {
    // clang-format off
    if constexpr (std::is_same_v<T, Image>) { return LoadImage(path); }
    else if constexpr (std::is_same_v<T, ShaderSource>) { return LoadShaderSource(path); }
    else if constexpr (std::is_same_v<T, Texture>) { return LoadTexture(path); }
    // No ShaderProgram here. It's implicitly made from Material loader
    else if constexpr (std::is_same_v<T, Material>) { return LoadMaterial(path); }
    else {
      HW_CORE_ERROR("Tried to load unsupported resource type.");
      return nullptr;
    }
    // clang-format on
  };

  AssetLibrary<Image> imgs_;
  AssetLibrary<Texture> textures_;
  AssetLibrary<ShaderSource> shader_srcs_;
  AssetLibrary<ShaderProgram> shaders_;
  AssetLibrary<Material> material_prefabs_; // "Default" instances from file
  std::vector<SharedPtr<Material>> material_instances_; // Actual instances

private:
  Renderer *renderer_{};
  //
  SharedPtr<Image> LoadImage(const std::filesystem::path &path);
  SharedPtr<ShaderSource> LoadShaderSource(const std::filesystem::path &path);
  SharedPtr<Texture> LoadTexture(const std::filesystem::path &path);
  SharedPtr<ShaderProgram> LoadShaderProgram(const std::string &concat_name);
  SharedPtr<Material> LoadMaterial(const std::filesystem::path &path);
};

} // namespace weather
