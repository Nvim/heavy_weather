#pragma once

#include "Event.hpp"
#include "EventSystem.hpp"

namespace weather {
class WidgetCloseEvent : public Event {
public:
  WidgetCloseEvent(u64 id) : id_{id} {}

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_WIDGET_CLOSED;

  u64 GetID() const { return id_; }

private:
  u64 id_;
};
} // namespace weather
