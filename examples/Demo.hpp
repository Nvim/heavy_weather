#pragma once
#include "heavy_weather/core/Application.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/EventCallback.hpp"
#include "heavy_weather/event/KeyPressedEvent.hpp"
#include "heavy_weather/event/MouseMovedEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Gui/Gui.hpp"
#include "heavy_weather/rendering/Mesh.hpp"
#include "heavy_weather/rendering/Pipeline.hpp"
#include "heavy_weather/rendering/Renderer.hpp"

using weather::KeyPressedEvent;
using weather::MouseMovedEvent, weather::EventCallback, weather::EventRegister;

class Demo : public weather::Application {
public:
  Demo(weather::WindowProps &window_props, f64 fps);
  ~Demo() override;

private:
  void OnMouseMoved(const MouseMovedEvent &e);
  void OnKeyPressed(const KeyPressedEvent &evt);
  void OnResize(const weather::ResizeEvent &e);
  void OnRender(f64 delta) override;

  void InitGraphics();

  //
  EventCallback<MouseMovedEvent> mouse_callback_;
  UniquePtr<weather::graphics::Mesh> square_{nullptr};
  UniquePtr<weather::graphics::Mesh> tri_{nullptr};
  UniquePtr<weather::graphics::Pipeline> pipeline_{nullptr};
  weather::graphics::Renderer renderer_;
  weather::graphics::Gui gui_;
  glm::vec4 bgcolor_{1.0f, 0.2f, 0.9f, 1.0f};
};
