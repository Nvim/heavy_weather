/*
 * Wraps a callback to run on a specific event
 * - Stores which event type is associated with it with template
 * - Asserts that the event we try to run on has good type
 */

#pragma once
#include "Event.hpp"
// #include "EventSystem.hpp"

#include <functional>
#include <string>

namespace weather {

template <typename EventType>
using EventCallback = std::function<void(const EventType &e)>;

// Interface
class EventCallbackWrapperInterface {
public:
  virtual void Trigger(const Event &e) const = 0;
  virtual const std::string &GetID() const = 0;
  virtual ~EventCallbackWrapperInterface() = default;
};

// Templated class
template <typename EventType>
class EventCallbackWrapper : public EventCallbackWrapperInterface {
public:
  explicit EventCallbackWrapper(const EventCallback<EventType> &callback)
      : name_(callback.target_type().name()), callback_(callback) {}

  void Trigger(const Event &e) const override {
    if (e.GetEvtCode() == EventType::kCode) {
      callback_(static_cast<const EventType &>(e));
    }
  }
  const std::string &GetID() const override { return name_; }
  ~EventCallbackWrapper() override = default;

private:
  std::string name_;
  EventCallback<EventType> callback_;
};

} // namespace weather
