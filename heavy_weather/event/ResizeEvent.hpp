#pragma once

#include "Event.hpp"
#include "EventSystem.hpp"
#include "heavy_weather/engine.h"

// TODO: this assumes a single window
namespace weather {
using size = struct {
  u16 w;
  u16 h;
};

class ResizeEvent : public Event {
public:
  ResizeEvent(u16 w, u16 h, u16 old_w, u16 old_h)
      : new_sz{w, h}, old_sz{old_w, old_h} {}

  ResizeEvent(size nw, size old) : new_sz{nw}, old_sz{old} {}

  EventCode GetEvtCode() const override { return CODE; }
  static constexpr EventCode CODE = EventCode::EVENT_RESIZED;

  const size &NewSize() const { return new_sz; }

  const size &OldSize() const { return old_sz; }

private:
  size new_sz;
  size old_sz;
};
} // namespace weather
