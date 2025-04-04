/*
 * Cool helpers to work with events without being a template wizard.
 * Not cool assumption of a global singleton EventSystem.
 */
#pragma once

#include "EventCallback.hpp"
#include "EventSystem.hpp"

namespace weather {

template <typename EvtType>
inline void EventRegister(EventCallback<EvtType> &func, void* instance) {
  std::unique_ptr<EventCallbackWrapperInterface> ptr =
      std::make_unique<EventCallbackWrapper<EvtType>>(func, instance);

  EventSystem::Get().Register(EvtType::kCode, std::move(ptr));
}

template <typename EvtType>
inline void EventUnregister(EventCallback<EvtType> &func) {
  // construct wrapper so we're safe if we change getID's implementation
  auto wrapper = EventCallbackWrapper<EvtType>{func};
  const std::string name = wrapper.GetID();
  void* instance = wrapper.GetInstance();
  EventSystem::Get().Unregister(EvtType::kCode, name, instance);
}

inline void EventDispatch(Event &&e) {
  EventSystem::Get().DispatchEvent(std::move(e));
}

} // namespace weather
