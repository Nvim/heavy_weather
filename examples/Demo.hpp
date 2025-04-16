#pragma once
#include "heavy_weather/core/Application.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/engine.h"
#include "heavy_weather/event/EventCallback.hpp"
#include "heavy_weather/event/KeyPressedEvent.hpp"
#include "heavy_weather/event/MouseMovedEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Renderer.hpp"
#include "heavy_weather/resources/AssetManager.hpp"
#include "heavy_weather/scene/Scene.hpp"

using weather::KeyPressedEvent;
using weather::MouseMovedEvent, weather::EventCallback, weather::EventRegister;

class Demo : public weather::Application {
public:
  Demo(weather::WindowProps &window_props, f64 fps,
       weather::graphics::RendererInitParams &render_params);
  ~Demo() override;
  const char *GetProgramName() const override;

private:
  void OnMouseMoved(const MouseMovedEvent &e);
  void OnKeyPressed(const KeyPressedEvent &evt);
  void OnResize(const weather::ResizeEvent &e);
  void OnRender(f64 delta) override;
  void OnGuiRender(f64 delta) override;

  void InitGraphics();

  //
  EventCallback<MouseMovedEvent> mouse_callback_;
  weather::graphics::Renderer renderer_;
  weather::AssetManager asset_mgr_{&renderer_};

  weather::graphics::Scene scene_manager_;
  glm::vec4 bgcolor_{0.15f, 0.15f, 0.15f, 1.0f};
};
