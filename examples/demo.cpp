#include "Demo.hpp"
#include "heavy_weather/core/Entry.hpp"
#include "heavy_weather/core/Input/InputSystem.hpp"
#include "heavy_weather/core/Input/KeyCodes.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include <GLFW/glfw3.h>
#include <functional>

// Shortcut for registering events
#define BIND_EVENT_FUNC(func) std::bind(func, this, std::placeholders::_1)

// Entrypoint hook:
weather::Application *weather::CreateAppHook() { return new Demo{}; }

Demo::Demo() {
#ifdef PLATFORM_LINUX
  HW_APP_DEBUG("LINUX_PLATFORM");
#endif
  mouse_callback_ = [this](const MouseMovedEvent &e) { this->OnMouseMoved(e); };
  EventRegister(mouse_callback_);
  EventCallback<KeyPressedEvent> e = BIND_EVENT_FUNC(&Demo::OnKeyPressed);
  EventRegister(e);
}

Demo::~Demo() {
  EventUnregister(mouse_callback_);
  EventCallback<KeyPressedEvent> e = BIND_EVENT_FUNC(&Demo::OnKeyPressed);
  EventUnregister(e);
}

void Demo::OnMouseMoved(const MouseMovedEvent &e) {
  HW_APP_DEBUG("Mouse moved: ({}, {})", e.Position().x, e.Position().y);
}

void Demo::OnKeyPressed(const KeyPressedEvent &evt) {
  i32 action = evt.Action();
  i32 key = evt.KeyCode();
  if (action == GLFW_PRESS) {
    HW_CORE_TRACE("Key {} has been pressed", key);
    if (weather::InputSystem::isKeyDown(HW_KEY_O)) {
      HW_CORE_TRACE("Yay");
    }
    // HW_CORE_TRACE("Key O status: {}",
  } else {
    HW_CORE_TRACE("Key {} has been released", key);
  }
  if (key == GLFW_KEY_ESCAPE) {
    HW_CORE_DEBUG("Shutdown keybind has been pressed.");
    EventDispatch(weather::WindowCloseEvent{(void *)&this->GetWindow()});
  }
}
