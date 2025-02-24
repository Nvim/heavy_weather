#pragma once

#include <heavy_weather/rendering/Gui/GuiComponent.hpp>
#include <heavy_weather/rendering/Gui/IWidget.hpp>

namespace weather::graphics {
// Widget used to control a mesh
// TODO: generalize to any scene node
class MeshWidget : public IWidget {
public:
  void Render() const override;
  explicit MeshWidget(const char *name, GuiComponentDesc &&c,
                      GuiComponentDesc &&t, GuiComponentDesc &&s,
                      GuiComponentDesc &&r, GuiComponentDesc&& d);
  ~MeshWidget() override = default;

  // No copy construct/assign here, 1 mesh => 1 widget.
  MeshWidget(const MeshWidget &) = default;
  MeshWidget(MeshWidget &&) = delete;
  MeshWidget &operator=(const MeshWidget &) = default;
  MeshWidget &operator=(MeshWidget &&) = delete;

private:
  const char *name_;
  GuiComponent<Color4> color_;
  GuiComponent<SliderFloat> translation_;
  GuiComponent<SliderFloat> scale_;
  GuiComponent<SliderFloat> rotation_;
  GuiComponent<Button> delete_;
};
} // namespace weather::graphics
