#include "AssetManager.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "imgui.h"
#include <fstream>
#include <glm/detail/qualifier.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace weather {

using Texture = graphics::Texture;
using Material = graphics::Material;
using ShaderProgram = graphics::ShaderProgram;
using Renderer = graphics::Renderer;

static bool ValidateMaterialJSON(const json &data);

AssetManager::AssetManager(Renderer *renderer) : renderer_{renderer} {
  EventCallback<GuiRenderEvent> c = [this](const GuiRenderEvent &e) {
    this->OnGuiRender(e);
  };
  EventRegister(c, this);
}

void AssetManager::OnGuiRender(const GuiRenderEvent &evt) {
  (void)evt;
  ImGui::Begin("Assets");
  shader_srcs_.OnGuiRender();
  imgs_.OnGuiRender();
  textures_.OnGuiRender();
  shaders_.OnGuiRender();
  material_prefabs_.OnGuiRender();
  ImGui::End();
}

/*
 * 'Primitive' types can be loaded by the AssetLibrary using static Loader.
 * Just forward the call:
 */
SharedPtr<Image> AssetManager::LoadImage(const std::filesystem::path &path) {
  return imgs_.Load(path);
}
SharedPtr<ShaderSource>
AssetManager::LoadShaderSource(const std::filesystem::path &path) {
  return shader_srcs_.Load(path);
}

/*
 * These asset types require GPU resource creation and are aggregates of other
 * assets/resources. They require more work:
 */
SharedPtr<graphics::Texture>
AssetManager::LoadTexture(const std::filesystem::path &path) {
  if (textures_.Has(path)) {
    return textures_.Get(path);
  }
  HW_CORE_DEBUG("AssetManager: Creating new Texture for path `{}`.",
                path.string());
  auto img = imgs_.Load(path.string());
  HW_ASSERT(img != nullptr);
  auto tex = renderer_->CreateTexture(img);
  textures_.Add(tex);
  return textures_.Get(Texture::ComputeName(path));
}

SharedPtr<Material>
AssetManager::LoadMaterial(const std::filesystem::path &path) {
  SharedPtr<Material> m = material_prefabs_.GetPath(path);
  if (m != nullptr) {
    // Skip loading from file if we already loaded the file once:
    SharedPtr<Material> mm = std::make_shared<Material>(*m);
    HW_ASSERT(mm->Path() == path);
    // mm->SetPath(path);
    return mm;
  }
  HW_CORE_DEBUG("AssetManager: Creating new Material for path `{}`.",
                path.string());
  std::ifstream f{path};
  json data = json::parse(f);
  if (!ValidateMaterialJSON(data)) {
    return nullptr;
  }

  auto name = data["name"].get<std::string>();
  auto vertex_str = data["vertex"].get<std::string>();
  auto fragment_str = data["fragment"].get<std::string>();

  // Create shader program: (TODO: check shaders_ for concatenated name before)
  SharedPtr<ShaderProgram> pipeline{nullptr};
  const std::string concat_name = ShaderProgram::CreateName(
      std::filesystem::path(vertex_str), std::filesystem::path(fragment_str));
  if (shaders_.Has(concat_name)) {
    pipeline = shaders_.Get(concat_name);
  } else {
    HW_CORE_DEBUG("AssetManager: Creating new ShaderProgram for concat `{}`.",
                  concat_name);
    auto vs = shader_srcs_.Load(vertex_str);
    auto fs = shader_srcs_.Load(fragment_str);
    HW_ASSERT(vs != nullptr && fs != nullptr);
    pipeline = renderer_->CreatePipeline(vs, fs);
    shaders_.Add(pipeline);
  }
  HW_ASSERT(pipeline != nullptr);

  // Create material and set each uniform:
  m = std::make_shared<Material>(pipeline, std::move(name), path);
  HW_ASSERT(m->Path() == path);

  if (data.contains("textures")) {
    std::unordered_map<std::string, std::string> textures = data["textures"];
    for (const auto &kv : textures) {
      SharedPtr<Texture> tex{nullptr};
      if (textures_.Has(Texture::ComputeName(kv.second))) {
        tex = textures_.Get(Texture::ComputeName(kv.second));
      } else {
        tex = LoadResource<Texture>(kv.second);
        HW_ASSERT(textures_.Has(Texture::ComputeName(kv.second)));
      }
      HW_ASSERT(tex != nullptr);
      m->SetUniformValue<SharedPtr<graphics::Texture>>(kv.first.c_str(), tex);
    }
  }

  if (data["uniforms"].contains("ints") &&
      data["uniforms"]["ints"].is_object()) {
    std::unordered_map<std::string, i32> ints = data["uniforms"].at("ints");
    for (const auto &kv : ints) {
      m->SetUniformValue<i32>(kv.first.c_str(), kv.second);
    }
  }
  if (data["uniforms"].contains("floats") &&
      data["uniforms"]["floats"].is_object()) {
    std::unordered_map<std::string, f32> floats = data["uniforms"].at("floats");
    for (const auto &kv : floats) {
      m->SetUniformValue<f32>(kv.first.c_str(), kv.second);
    }
  }
  if (data["uniforms"].contains("float2s") &&
      data["uniforms"]["float2s"].is_object()) {
    std::unordered_map<std::string, std::array<f32, 2>> f2s =
        data["uniforms"].at("float2s");
    for (const auto &kv : f2s) {
      m->SetUniformValue(kv.first.c_str(), kv.second);
    }
  }
  if (data["uniforms"].contains("float3s") &&
      data["uniforms"]["float3s"].is_object()) {
    std::unordered_map<std::string, std::array<f32, 3>> f3s =
        data["uniforms"].at("float3s");
    for (const auto &kv : f3s) {
      m->SetUniformValue<std::array<f32, 3>>(kv.first.c_str(), kv.second);
    }
  }
  if (data["uniforms"].contains("float4s") &&
      data["uniforms"]["float4s"].is_object()) {
    std::unordered_map<std::string, std::array<f32, 4>> f4s =
        data["uniforms"].at("float4s");
    for (const auto &kv : f4s) {
      m->SetUniformValue<std::array<f32, 4>>(kv.first.c_str(), kv.second);
    }
  }
  // TODO: matrices
  material_instances_.push_back(std::make_shared<Material>(*m));
  material_prefabs_.Add(m);
  return material_instances_.back();
}

static bool ValidateMaterialJSON(const json &data) {
  if (!data.contains("name") || !data.contains("fragment") ||
      !data.contains("vertex") || !data.contains("uniforms")) {
    HW_CORE_ERROR("Couldn't parse material file: missing fields");
    return false;
  }
  if (!data["vertex"].is_string() || !data["fragment"].is_string() ||
      !data["name"].is_string() || !data["uniforms"].is_object()) {
    HW_CORE_ERROR("Couldn't parse material file: bad type");
    return false;
  }
  if (data["uniforms"].contains("ints") &&
      !data["uniforms"]["ints"].is_object()) {
    HW_CORE_ERROR("Couldn't parse material file: bad type for field `ints`");
    return false;
  }
  if (data["uniforms"].contains("floats") &&
      !data["uniforms"]["floats"].is_object()) {
    HW_CORE_ERROR("Couldn't parse material file: bad type for field `floats`");
    return false;
  }
  if (data["uniforms"].contains("float2s") &&
      !data["uniforms"]["float2s"].is_object()) {
    HW_CORE_ERROR("Couldn't parse material file: bad type for field `float2s`");
    return false;
  }
  if (data["uniforms"].contains("float3s") &&
      !data["uniforms"]["float3s"].is_object()) {
    HW_CORE_ERROR("Couldn't parse material file: bad type for field `float3s`");
    return false;
  }
  return true;
}

} // namespace weather
