#pragma once

#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include "imgui.h"

namespace weather::graphics {

// enum class GuiComponentType : u8 {
//   FloatSlider,
//   Float2Slider,
//   Float3Slider,
//   Float4Slider,
//   Color3Piker,
//   Color4Picker,
// };

struct GuiComponentDesc {
  // GuiComponentType type;
  void *data = nullptr;
  f32 min = 0.0f;
  f32 max = 10.0f;
  const char *name;
  std::function<void()> callback = nullptr;
};

// Template param D is a Draw "Strategy"/"Behavior" obtained with inheritence
// Components don't need an id as they're owned by a widget
template <typename D> struct GuiComponent : public D {
private:
  GuiComponentDesc desc_;

public:
  // u64 id;
  GuiComponent(GuiComponentDesc &&desc) : desc_{std::move(desc)} {}

  void Render() const {
    if (this->Draw(desc_) && desc_.callback) {
      desc_.callback();
    }
  }
};

/* COMPONENTS DRAW BEHAVIOR: Pass these as template parameters to GuiComponent
 */
struct Button {
  static bool Draw(const GuiComponentDesc &desc) {
    return (ImGui::Button(desc.name));
  }
};

struct SliderFloat {
  static bool Draw(const GuiComponentDesc &desc) {
    return (
        ImGui::SliderFloat(desc.name, (float *)desc.data, desc.min, desc.max));
  }
};

struct SliderFloat2 {
  static bool Draw(const GuiComponentDesc &desc) {
    return (
        ImGui::SliderFloat2(desc.name, (float *)desc.data, desc.min, desc.max));
  }
};

struct SliderFloat3 {
  static bool Draw(const GuiComponentDesc &desc) {
    return (
        ImGui::SliderFloat3(desc.name, (float *)desc.data, desc.min, desc.max));
  }
};

struct SliderFloat4 {
  static bool Draw(const GuiComponentDesc &desc) {
    return (
        ImGui::SliderFloat4(desc.name, (float *)desc.data, desc.min, desc.max));
  }
};

struct Color4 {
  static bool Draw(const GuiComponentDesc &desc) {
    return (ImGui::ColorEdit4(desc.name, (float *)desc.data));
  }
};

} // namespace weather::graphics
