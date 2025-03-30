#pragma once

#include "Event.hpp"
#include "EventSystem.hpp"

namespace weather {
class GuiRenderEvent : public Event {
public:
  GuiRenderEvent() = default;

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_GUI_RENDER;
};
} // namespace weather
