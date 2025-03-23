#pragma once

#include "heavy_weather/ecs/ecs.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include <functional>

namespace weather::graphics {
/*
 * A Widget is a collection of method that calls Gui class function to render a
 * group of ImGui elements (buttons, sliders, etc), and is associated  with an
 * entity from the scene graph (ecs).
 * In order to get the data to bind to the ImGui elements, Widgets will query
 * the ECS on every call. This avoids "pre-baked" data that could be moved or go
 * out of scope later (vector realloc for example).
 * For uniformity, widgets are expected to take the form of an
 * ImGUI dropdown element (`ImGui::CollapsingHeader`), but this isn't forced in
 * by GUI system.
 * */

using WidgetFunc = std::function<void(Gui &, ECS &, u32)>;

// Associates an entity with a Gui widget
struct WidgetComponent {
  std::vector<WidgetFunc> funcs;
};

// WidgetFuncs:

void TransformControl(Gui &gui, ECS &scene, u32 entity);
void MaterialEditor(Gui &gui, ECS &scene, u32 entity);
void DeleteButton(Gui &gui, ECS &scene, u32 entity);

} // namespace weather::graphics
