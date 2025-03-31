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
    if (ImGui::Button("Reload")) {
      HW_CORE_DEBUG("Reloading shader {}", s.Path().string());
    }
    ImGui::TreePop();
  }
}

static void DisplayImage(const Image &i, i64 use_count) {
  if (ImGui::TreeNode(i.Name().c_str())) {
    ImGui::Text("Use count: %ld", use_count);
    ImGui::Text("%dx%d. %d Channels.", i.Size().first, i.Size().second,
                i.Channels());
    if (ImGui::Button("Reload")) {
      HW_CORE_DEBUG("Reloading image {}", i.Path().string());
    }
    ImGui::TreePop();
  }
}

static void DisplayShaderPipeline(const graphics::ShaderProgram &p,
                                  i64 use_count) {
  if (ImGui::TreeNode(p.Name().c_str())) {
    ImGui::Text("Use count: %ld", use_count);
    ImGui::Text("Vertex Shader: %s", p.VertexShader().Path().c_str());
    ImGui::Text("Fragment Shader: %s", p.FragmentShader().Path().c_str());
    DisplayShaderSource(p.VertexShader().Source(), use_count);
    DisplayShaderSource(p.FragmentShader().Source(), use_count);
    if (ImGui::Button("Reload")) {
      HW_CORE_DEBUG("Reloading shader pipeline {}", p.Name());
    }
    ImGui::TreePop();
  }
}

template <> void AssetLibrary<ShaderSource>::OnGuiRender() {
  if (ImGui::TreeNode("Shader Sources")) {
    for (auto &src : assets_) {
      DisplayShaderSource(*src.second, src.second.use_count());
    }
    ImGui::TreePop();
  }
}
template <> void AssetLibrary<weather::Image>::OnGuiRender() {
  if (ImGui::TreeNode("Images")) {
    for (const auto &img : assets_) {
      DisplayImage(*img.second, img.second.use_count());
    }
    ImGui::TreePop();
  }
}

template <> void AssetLibrary<weather::graphics::Texture>::OnGuiRender() {
  if (ImGui::TreeNode("Textures")) {
    for (const auto &t : assets_) {
      if (ImGui::TreeNode(t.first.c_str())) {
        ImGui::Text("Use count: %ld", t.second.use_count());
        DisplayImage(*t.second->img_, t.second->img_.use_count());
        ImGui::Image(t.second->Handle(),
                     {static_cast<float>(t.second->Size().first),
                      static_cast<float>(t.second->Size().second)});
        if (ImGui::Button("Reload")) {
          HW_CORE_DEBUG("Reloading texture {}", t.second.get()->Name());
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
}

template <> void AssetLibrary<weather::graphics::ShaderProgram>::OnGuiRender() {
  if (ImGui::TreeNode("Shader pipelines")) {
    for (const auto &p : assets_) {
      DisplayShaderPipeline(*p.second, p.second.use_count());
    }
    ImGui::TreePop();
  }
}

template <> void AssetLibrary<weather::graphics::Material>::OnGuiRender() {
  if (ImGui::TreeNode("Materials")) {
    for (const auto &mat : assets_) {
      if (ImGui::TreeNode(mat.first.c_str())) {
        ImGui::Text("Use count: %ld", mat.second.use_count());
        DisplayShaderPipeline(*mat.second->GetShader(), mat.second.use_count());
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
          HW_CORE_DEBUG("Reloading material {}", mat.second.get()->Name());
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
}

} // namespace weather
