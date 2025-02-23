/*
 * A Widget is a grouping of components. Every widget implements it's own 'Draw'
 * method, which will be called by GUI subsystem during rendering.
 * For uniformity, widgets are expected to take the form of an ImGUI dropdown
 * element (`ImGui::CollapsingHeader`), but this isn't forced in by GUI
 * system.
 * */
#pragma once

#include <heavy_weather/engine.h>
namespace weather::graphics {
// Interface used to store Widget in a collection
class IWidget {
private:
  u64 id_{};

public:
  virtual void Render() const = 0;
  u64 GetID() const { return id_; }
  void SetID(u64 id) { id_ = id; }
  IWidget() = default;

  IWidget(const IWidget &) = default;
  IWidget(IWidget &&) = default;
  IWidget &operator=(const IWidget &) = default;
  IWidget &operator=(IWidget &&) = default;
  virtual ~IWidget() = default;
};
} // namespace weather::graphics
