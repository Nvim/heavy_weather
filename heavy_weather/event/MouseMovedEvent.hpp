#pragma once

#include "Event.hpp"
#include "EventSystem.hpp"
#include "heavy_weather/engine.h"

// TODO: this assumes a single window
namespace weather {
using pos = struct {
  f64 x;
  f64 y;
};

class MouseMovedEvent : public Event {
public:
  MouseMovedEvent(pos new_pos) : pos_{new_pos} {}
  MouseMovedEvent(f64 x, f64 y) : pos_{x, y} {}

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_MOUSE_MOVED;

  const pos &Position() const { return pos_; }

private:
  pos pos_;
};
} // namespace weather
