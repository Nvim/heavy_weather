#include "WidgetComponent.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Material.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"

namespace weather::graphics {
void TransformControl(Gui &gui, ECS &scene, u32 entity) {
  (void)gui;
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

void DeleteEntityButton(Gui &gui, ECS &scene, u32 entity) {
  (void)gui;
  (void)scene;
  if (Gui::DrawButton("delete")) {
    EventDispatch(EntityRemovedEvent{entity});
    // scene.DestroyEntity(entity);
  }
}

#define GUI_PICKER(type, imgui_func)                                           \
  for (auto it = m.material->GetRange<type>().first;                           \
       it != m.material->GetRange<type>().second; it++) {                      \
    imgui_func(it->first.c_str(), (f32 *)&it->second);                         \
  }

void MaterialEditor(Gui &gui, ECS &scene, u32 entity) {
  (void)gui;
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
