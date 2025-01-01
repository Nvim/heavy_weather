#pragma once

#include "Event.hpp"
#include "EventSystem.hpp"

namespace weather {
class WindowCloseEvent : public Event {
public:
  WindowCloseEvent(void *window) : window_ptr_{window} {}

  EventCode GetEvtCode() const override { return CODE; }
  static constexpr EventCode CODE = EventCode::EVENT_WINDOW_CLOSED;

  const void *GetWindow() const { return window_ptr_; }

private:
  void *window_ptr_;
};
} // namespace weather
