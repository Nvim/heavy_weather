/*
 * Wraps a callback to run on a specific event
 * - Stores which event type is associated with it with template
 * - Asserts that the event we try to run on has good type
 */

#pragma once
#include "Event.hpp"
#include "heavy_weather/core/Logger.hpp"
// #include "EventSystem.hpp"

#include <functional>
#include <string>

namespace weather {

template <typename EventType>
using EventCallback = std::function<void(const EventType &e)>;

// Interface
class EventCallbackWrapperInterface {
public:
  // void Trigger(const Event &e) {
  //   Call(e);
  //   // HW_CORE_INFO("hey");
  // }
  virtual void Trigger(const Event &e) const = 0;
  virtual const std::string &GetID() const = 0;
  virtual ~EventCallbackWrapperInterface() = default;

private:
  // virtual void Call(const Event &e) = 0;
};

// Templated class
template <typename EventType>
class EventCallbackWrapper : public EventCallbackWrapperInterface {
public:
  explicit EventCallbackWrapper(const EventCallback<EventType> &callback) {
    callback_ = callback;
    name_ = callback.target_type().name();
  }

  void Trigger(const Event &e) const override {
    if (e.GetEvtCode() == EventType::CODE) {
      callback_(static_cast<const EventType &>(e));
    }
  }
  const std::string &GetID() const override { return name_; }
  ~EventCallbackWrapper() = default;

private:
  std::string name_;
  EventCallback<EventType> callback_;
  // void Call(const Event &e) override {
  //   HW_CORE_INFO("Call?");
  // if (e.GetEvtCode() == EventType::CODE) {
  //   HW_CORE_INFO("Event code matches code expected by callback.
  //   Executing."); callback_(static_cast<const EventType &>(e));
  // }
  // }
};

} // namespace weather
