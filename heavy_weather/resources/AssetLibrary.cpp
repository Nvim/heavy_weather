#include "AssetLibrary.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/resources/Image.hpp"
#include "heavy_weather/resources/ShaderSource.hpp"
#include "imgui.h"

namespace weather {

static void DisplayShaderSource(const ShaderSource &s, i64 use_count) {
  if (ImGui::TreeNode(s.Path().string().c_str())) {
    ImGui::Text("Use count: %ld", use_count);
    ImGui::Text("%s", s.Data().c_str());
    ImGui::TreePop();
  }
}

static void DisplayImage(const Image &i, i64 use_count) {
  if (ImGui::TreeNode(i.Name().c_str())) {
    ImGui::Text("Use count: %ld", use_count);
    ImGui::Text("%dx%d. %d Channels.", i.Size().first, i.Size().second,
                i.Channels());
    ImGui::TreePop();
  }
}

static void DisplayShaderPipeline(graphics::ShaderProgram &p, i64 use_count) {
  if (ImGui::TreeNode(p.Name().c_str())) {
    ImGui::Text("Use count: %ld", use_count);
    ImGui::Text("Vertex Shader: %s", p.VertexShader().Path().c_str());
    ImGui::Text("Fragment Shader: %s", p.FragmentShader().Path().c_str());
    DisplayShaderSource(p.VertexShader().Source(), use_count);
    DisplayShaderSource(p.FragmentShader().Source(), use_count);
    if (ImGui::Button("Reload")) {
      HW_CORE_DEBUG("Reloading shader pipeline {}", p.Name());
      p.Reload();
    }
    ImGui::TreePop();
  }
}

template <>
void AssetLibrary<ShaderSource>::OnGuiRender() {
  if (ImGui::TreeNode("Shader Sources")) {
    for (auto &src : assets_) {
      DisplayShaderSource(*src.second, src.second.use_count());
    }
    ImGui::TreePop();
  }
}
template <>
void AssetLibrary<weather::Image>::OnGuiRender() {
  if (ImGui::TreeNode("Images")) {
    for (const auto &img : assets_) {
      DisplayImage(*img.second, img.second.use_count());
    }
    ImGui::TreePop();
  }
}

template <>
void AssetLibrary<weather::graphics::Texture>::OnGuiRender() {
  if (ImGui::TreeNode("Textures")) {
    f32 max_x = 150, max_y = 150;
    for (const auto &t : assets_) {
      if (ImGui::TreeNode(t.first.c_str())) {
        ImGui::Text("Use count: %ld", t.second.use_count());
        f32 tex_x = static_cast<f32>(t.second->Size().first);
        f32 tex_y = static_cast<f32>(t.second->Size().second);
        DisplayImage(*t.second->img_, t.second->img_.use_count());
        ImGui::Image(t.second->Handle(),
                     {std::min(max_x, tex_x), std::min(max_y, tex_y)});
        if (ImGui::Button("Reload")) {
          HW_CORE_DEBUG("Reloading texture {}", t.second->Name());
          t.second->Reload();
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
}

template <>
void AssetLibrary<weather::graphics::ShaderProgram>::OnGuiRender() {
  if (ImGui::TreeNode("Shader pipelines")) {
    for (const auto &p : assets_) {
      DisplayShaderPipeline(*p.second, p.second.use_count());
    }
    ImGui::TreePop();
  }
}

template <>
void AssetLibrary<weather::graphics::Material>::OnGuiRender() {
  if (ImGui::TreeNode("Materials")) {
    for (const auto &mat : assets_) {
      if (ImGui::TreeNode(mat.first.c_str())) {
        ImGui::Text("Use count: %ld", mat.second->GetInstanceCount());
        DisplayShaderPipeline(*mat.second->GetShader(),
                              mat.second->GetShader().use_count());
        for (const auto &tex : mat.second->GetTextures()) {
          if (ImGui::TreeNode(tex.first.c_str())) {
            ImGui::Image(tex.second->Handle(),
                         {static_cast<float>(tex.second->Size().first),
                          static_cast<float>(tex.second->Size().second)});
            ImGui::TreePop();
          }
        }
        ImGui::Text("Uniforms: TODO");
        if (ImGui::Button("Reload")) {
          /* *
           * TODO: Specify what should be done here:
           * - Reload prefab only, or all instances as well?
           * - Dispatch reload to shaders/textures ?
           * */
          HW_CORE_DEBUG("Reloading material {} [NOT IMPLEMENTED]",
                        mat.second->Name());
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
}

} // namespace weather
