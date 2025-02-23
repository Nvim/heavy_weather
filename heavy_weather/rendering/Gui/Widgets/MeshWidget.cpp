#include "MeshWidget.hpp"
#include "heavy_weather/rendering/Gui/GuiComponent.hpp"
#include "imgui.h"

namespace weather::graphics {

MeshWidget::MeshWidget(const char *name, GuiComponentDesc &&c,
                       GuiComponentDesc &&t, GuiComponentDesc &&s,
                       GuiComponentDesc &&r)
    : name_{name}, color_{GuiComponent<Color4>{std::move(c)}},
      translation_{GuiComponent<SliderFloat>{std::move(t)}},
      scale_{GuiComponent<SliderFloat>{std::move(s)}},
      rotation_{GuiComponent<SliderFloat>{std::move(r)}} {}

void MeshWidget::Render() const {
  if (ImGui::TreeNode(name_)) {
    color_.Render();
    translation_.Render();
    scale_.Render();
    rotation_.Render();
    ImGui::TreePop();
  }
}

} // namespace weather::graphics
