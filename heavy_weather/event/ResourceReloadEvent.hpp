#pragma once

/*
 * Notifies that a Resource (disk data, or GPU) has been reloaded.
 * */

#include "EventSystem.hpp"
#include "heavy_weather/event/Event.hpp"

namespace weather {

template <typename T>
class ResourceReloadEvent : public Event {
public:
  ResourceReloadEvent(T* e) : resource_{e} {}

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_RESOURCE_RELOAD;

  T* GetResource() const {return resource_;}

private:
  T* resource_;
};

} // namespace weather
