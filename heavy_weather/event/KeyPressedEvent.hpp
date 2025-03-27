#pragma once

#include "Event.hpp"
#include "EventSystem.hpp"
#include "heavy_weather/engine.h"
#include "imgui.h"

namespace weather {
class KeyPressedEvent : public Event {
public:
  KeyPressedEvent(i32 keycode, i32 action)
      : keycode_{keycode}, action_{action} {
#ifdef HW_ENABLE_GUI
    handled = ImGui::GetIO().WantCaptureKeyboard;
#endif
  }

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_KEY_PRESSED;

  i32 KeyCode() const { return keycode_; }
  i32 Action() const { return action_; }

private:
  i32 keycode_;
  i32 action_;
};
} // namespace weather
