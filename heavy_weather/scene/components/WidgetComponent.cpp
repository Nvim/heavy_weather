#include "WidgetComponent.hpp"
#include "heavy_weather/event/EntityRemoved.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/MaterialComponent.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"

namespace weather::graphics {
void TransformControl(Gui &gui, ECS &scene, u32 entity) {
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

void MaterialPicker(Gui &gui, ECS &scene, u32 entity) {
  if (Gui::BeginTreeNode("Material")) {
    Gui::DrawColorEdit4(
        "color", &scene.GetComponentPtr<MaterialComponent>(entity)->color);
    Gui::EndTreeNode();
  }
}

void DeleteButton(Gui &gui, ECS &scene, u32 entity) {
  if (Gui::DrawButton("delete")) {
    EventDispatch(EntityRemovedEvent{entity});
    // scene.DestroyEntity(entity);
  }
}

} // namespace weather::graphics
