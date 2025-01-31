#pragma once

#include "heavy_weather/engine.h"
#include "imgui.h"

namespace weather::graphics {

enum class GuiComponentType : u8 {
  FloatSlider,
  Float2Slider,
  Float3Slider,
  Float4Slider,
  Color3Piker,
  Color4Picker,
};

struct GuiComponentDesc {
  GuiComponentType type;
  void *data = nullptr;
  f32 min = 0.0f;
  f32 max = 10.0f;
  const char *name;
};

class GuiComponent {
public:
  GuiComponentDesc desc;
  u64 id;
  GuiComponent(GuiComponentDesc &desc, u64 id) : desc{desc}, id{id} {}

  void Render() {
    switch (desc.type) {
    case GuiComponentType::FloatSlider:
      ImGui::SliderFloat(desc.name, (float *)desc.data, desc.min, desc.max);
      break;
    case GuiComponentType::Float2Slider:
      ImGui::SliderFloat2(desc.name, (float *)desc.data, desc.min, desc.max);
      break;
    case GuiComponentType::Float3Slider:
      ImGui::SliderFloat3(desc.name, (float *)desc.data, desc.min, desc.max);
      break;
    case GuiComponentType::Float4Slider:
      ImGui::SliderFloat4(desc.name, (float *)desc.data, desc.min, desc.max);
      break;
    case GuiComponentType::Color3Piker:
      ImGui::ColorEdit3(desc.name, (float *)desc.data);
      break;
    case GuiComponentType::Color4Picker:
      ImGui::ColorEdit4(desc.name, (float *)desc.data);
    default:
      break;
    }
  }
};

} // namespace weather::graphics
// const char *Name() const { return desc.name; }
// f32 Min() const { return desc_.min; }
// f32 Max() const { return desc_.max; }
// SharedPtr<void> Data() const { return desc_.data; }
// u64 Id() const { return id_; }
//
// class Float3SliderComponent : GuiComponent {
// public:
//   Float3SliderComponent(GuiComponentDesc &&desc, u64 id)
//       : GuiComponent{std::move(desc), id} {}
//   void Render() override {
//     ImGui::SliderFloat3(Name(), (float*)Data().get(), Min(), Max());
//   }
// };
//
// class Float4SliderComponent : GuiComponent {
// public:
//   Float4SliderComponent(GuiComponentDesc &&desc, u64 id)
//       : GuiComponent{std::move(desc), id} {}
//   void Render() override {
//     ImGui::SliderFloat4(Name(), (float*)Data().get(), Min(), Max());
//   }
// };
//
// class Color3PickerComponent : GuiComponent {
// public:
//   Color3PickerComponent(GuiComponentDesc &&desc, u64 id)
//       : GuiComponent{std::move(desc), id} {}
//   void Render() override {
//     ImGui::ColorEdit3(Name(), (float*)Data().get());
//   }
// };
//
// class Color4PickerComponent : GuiComponent {
// public:
//   Color4PickerComponent(GuiComponentDesc &&desc, u64 id)
//       : GuiComponent{std::move(desc), id} {}
//   void Render() override {
//     ImGui::ColorEdit3(Name(), (float*)Data().get());
//   }
// };
