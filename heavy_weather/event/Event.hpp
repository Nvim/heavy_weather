#pragma once

namespace weather {

enum class EventCode;

struct Event {
protected:
  bool handled_{false};

public:
  virtual EventCode GetEvtCode() const = 0;
};
} // namespace weather
