#pragma once

/*
 * Notifies that an Entity from ECS has been deleted.
 * Used for delaying and batching removals to the end of the frame
 * */

#include "EventSystem.hpp"
#include "heavy_weather/event/Event.hpp"

namespace weather {

class EntityRemovedEvent : public Event {
public:
  EntityRemovedEvent(u32 id) : id_{id} {}

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_ENTITY_REMOVED;

  u64 GetID() const { return id_; }

private:
  u32 id_;
};

} // namespace weather
