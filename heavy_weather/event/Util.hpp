/*
 * Cool helpers to work with events without being a template wizard.
 * Not cool assumption of a global singleton EventSystem.
 */
#pragma once

#include "EventCallback.hpp"
#include "EventSystem.hpp"

namespace weather {

template <typename EvtType>
inline void EventRegister(EventCallback<EvtType> &func) {
  std::unique_ptr<EventCallbackWrapperInterface> ptr =
      std::make_unique<EventCallbackWrapper<EvtType>>(func);

  EventSystem::get().Register(EvtType::CODE, std::move(ptr));
}

template <typename EvtType>
inline void EventUnregister(EventCallback<EvtType> &func) {
  // construct wrapper so we're safe if we change getID's implementation
  auto wrapper = EventCallbackWrapper<EvtType>{func};
  const std::string name = wrapper.GetID();
  EventSystem::get().Unregister(EvtType::CODE, name);
}

inline void EventDispatch(Event &&e) {
  EventSystem::get().DispatchEvent(std::move(e));
}

} // namespace weather
