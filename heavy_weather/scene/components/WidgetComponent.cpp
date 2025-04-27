#include "WidgetComponent.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/LightSourceComponent.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"
#include "imgui.h"

namespace weather::graphics {
void TransformControl(ECS &scene, u32 entity) {
  auto *tr = scene.GetComponentPtr<TransformComponent>(entity);
  if (Gui::BeginTreeNode("Transform")) {
    if (Gui::DrawSliderFloat3("Translation", &tr->translation, -10.0f, 10.0f)) {
      tr->dirty = true;
    }
    if (Gui::DrawSliderFloat3("Rotation", &tr->rotation, 0.0f, 360.0f)) {
      tr->dirty = true;
    }
    if (Gui::DrawSliderFloat3("Scale", &tr->scale, 0.0f, 10.0f)) {
      tr->dirty = true;
    }
    Gui::EndTreeNode();
  }
}

void LightSourceControl(ECS &scene, u32 entity) {
  bool is_light = scene.HasComponent<LightSourceComponent>(entity);
  if (ImGui::Button("Toggle lightsource")) {
    if (is_light) {
      scene.RemoveComponent<LightSourceComponent>(entity);
      is_light = false;
    } else {
      LightSourceComponent c{};
      {
        c.position = scene.GetComponent<TransformComponent>(entity).translation;
        c.constant = 1.0f;
        c.ambient = {0.2f, 0.2f, 0.2f};
        c.linear = 0.14f;
        c.diffuse = {0.6f, 0.6f, 0.6f};
        c.quadratic = 0.07f;
        c.specular = {1.0f, 1.0f, 1.0f, 0.0f};
      }
      scene.AddComponent(entity, c);
      is_light = true;
    }
  }
  if (is_light) {
    auto *comp = scene.GetComponentPtr<LightSourceComponent>(entity);
    if (Gui::BeginTreeNode("Light Source")) {
      ImGui::ColorEdit3("Ambient", (f32 *)(&comp->ambient));
      ImGui::ColorEdit3("Diffuse", (f32 *)(&comp->diffuse));
      ImGui::ColorEdit3("Specular", (f32 *)(&comp->specular));
      ImGui::InputFloat("Constant", &comp->constant);
      ImGui::InputFloat("Linear", &comp->linear);
      ImGui::InputFloat("Quadratic", &comp->quadratic);
      Gui::EndTreeNode();
    }
  }
}

void DeleteEntityButton(ECS &scene, u32 entity) {
  (void)scene;
  if (Gui::DrawButton("delete")) {
    EventDispatch(EntityRemovedEvent{entity});
  }
}

#define GUI_PICKER(type, imgui_func)                                           \
  for (auto it = m.material->GetRange<type>().first;                           \
       it != m.material->GetRange<type>().second; it++) {                      \
    imgui_func(it->first.c_str(), (f32 *)&it->second);                         \
  }

void MaterialEditor(ECS &scene, u32 entity) {
  if (Gui::BeginTreeNode("Material")) {
    auto m = scene.GetComponent<MaterialComponent>(entity);

    // Int/Float pickers:
    for (auto it = m.material->GetRange<i32>().first;
         it != m.material->GetRange<i32>().second; it++) {
      Gui::DrawScalarInt(it->first.c_str(), &it->second, 1);
    }
    for (auto it = m.material->GetRange<f32>().first;
         it != m.material->GetRange<f32>().second; it++) {
      Gui::DrawScalarFloat(it->first.c_str(), &it->second, 1.0f);
    }
    GUI_PICKER(glm::vec2, Gui::DrawInputFloat2)
    GUI_PICKER(glm::vec3, Gui::DrawInputFloat3)
    GUI_PICKER(glm::vec4, Gui::DrawInputFloat4)
    // TODO: Matrix Editor
    // GUI_PICKER(glm::mat3, ImGui::InputFloat2)
    // GUI_PICKER(glm::mat4, ImGui::InputFloat2)

    // Texture picker:
    // TODO
    Gui::EndTreeNode();
  }
}
#undef GUI_PICKER

} // namespace weather::graphics
