#pragma once
#include "heavy_weather/core/Application.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/EventCallback.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include "heavy_weather/resources/AssetManager.hpp"
#include "heavy_weather/scene/Scene.hpp"

namespace weather {
class KeyPressedEvent;
class MouseMovedEvent;
namespace graphics {
class Scene;
class RenderTarget;
class MultiPassRenderTarget;
class Renderer;
} // namespace graphics
} // namespace weather

using namespace weather;

enum class Attachment : u8 {
  RGB,
  POSITION,
  NORMAL,
};

class Demo : public Application {
public:
  Demo(WindowProps &window_props, f64 fps,
       graphics::RendererInitParams &render_params);
  ~Demo() override;
  const char *GetProgramName() const override;

private:
  void OnMouseMoved(const MouseMovedEvent &e);
  void OnKeyPressed(const KeyPressedEvent &evt);
  void OnResize(const ResizeEvent &e);
  bool OnRender(f64 delta) override;
  bool OnGuiRender(f64 delta) override;

  void InitGraphics();

  //
  EventCallback<MouseMovedEvent> mouse_callback_;
  graphics::Renderer renderer_;
  AssetManager asset_mgr_{&renderer_};

  // SharedPtr<graphics::RenderTarget> rendertarget_;
  SharedPtr<graphics::MultiPassRenderTarget> rendertarget_;
  graphics::Scene scene_;
  glm::vec4 bgcolor_{0.15f, 0.15f, 0.15f, 1.0f};
  Attachment attachment_{Attachment::RGB};
};
