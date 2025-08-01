#pragma once

#include "heavy_weather/engine.h"
namespace weather {

enum class EventCode : u8;

struct Event {
protected:
public:
  bool handled{false};
  Event() = default;
  Event(const Event &) = default;
  Event(Event &&) = delete;
  Event &operator=(const Event &) = default;
  Event &operator=(Event &&) = delete;
  virtual ~Event() = default;
  virtual EventCode GetEvtCode() const = 0;
};
} // namespace weather
