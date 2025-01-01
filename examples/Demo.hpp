#pragma once
#include "heavy_weather/core/Application.hpp"
#include "heavy_weather/event/EventCallback.hpp"
#include "heavy_weather/event/KeyPressedEvent.hpp"
#include "heavy_weather/event/MouseMovedEvent.hpp"
#include "heavy_weather/event/Util.hpp"

using weather::KeyPressedEvent;
using weather::MouseMovedEvent, weather::EventCallback, weather::EventRegister;

class Demo : public weather::Application {
public:
  Demo();
  ~Demo();

private:
  void OnMouseMoved(const MouseMovedEvent &e);
  void OnKeyPressed(const KeyPressedEvent &evt);

private:
  EventCallback<MouseMovedEvent> mouse_callback_;
};
