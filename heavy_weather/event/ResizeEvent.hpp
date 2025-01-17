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
      : new_sz_{w, h}, old_sz_{old_w, old_h} {}

  ResizeEvent(size nw, size old) : new_sz_{nw}, old_sz_{old} {}

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_RESIZED;

  const size &NewSize() const { return new_sz_; }

  const size &OldSize() const { return old_sz_; }

private:
  size new_sz_;
  size old_sz_;
};
} // namespace weather
