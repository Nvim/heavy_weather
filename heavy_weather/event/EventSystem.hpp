/*
- define event types with unique codes
- event struct wraps code + handled status
- callback wrapper to wrap the call in a downcast + type check + (handled
check?)
- EventSystem: singleton to manage it all:
    - store a mapping of code->vector of callbacks
    - manage register/unregister operations
    - allow for queing and dispatching events
(dispatch = run the callback+set the handled flag to true)
*/

#pragma once
#include "EventCallback.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/engine.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace weather {

template <typename EventType>
using EventCallback = std::function<void(const EventType &e)>;

enum class EventCode : u8 {
  EVENT_WINDOW_CLOSED,
  EVENT_RESIZED,
  EVENT_KEY_PRESSED,
  EVENT_KEY_RELEASED,
  EVENT_BUTTON_PRESSED,
  EVENT_BUTTON_RELEASED,
  EVENT_MOUSE_MOVED,
  EVENT_ENTITY_REMOVED,
  EVENT_GUI_RENDER,
  EVENT_RESOURCE_RELOAD,
};

class EventCallbackWrapperInterface;

class EventSystem {
public:
  EventSystem(EventSystem &e) = delete;
  EventSystem(EventSystem &&e) = delete;
  EventSystem operator=(EventSystem &e) = delete;
  EventSystem operator=(EventSystem &&e) = delete;
  ~EventSystem() {
    map_.clear(); // clear is enough since map holds unique_ptrs
  }

  static EventSystem &Get() {
    if (s_instance == nullptr) {
      HW_CORE_INFO("Creating a new instance of eventsystem");
      s_instance = std::unique_ptr<EventSystem>(new EventSystem()); // NOLINT
    }
    return *s_instance;
  }

  static void Shutdown() {
    HW_CORE_DEBUG("Shutting down Event System.");
    s_instance = nullptr;
  }

  void Register(EventCode code,
                std::unique_ptr<EventCallbackWrapperInterface> &&callback) {
    // check if a callback is already registered for this code:
    auto entry = map_.find(code);
    if (entry != map_.end()) {
      auto &vec = entry->second;

      auto it = std::find_if(
          vec.begin(), vec.end(),
          [&callback](
              const std::unique_ptr<EventCallbackWrapperInterface> &elem) {
            return elem->GetID() == callback->GetID() &&
                   elem->GetInstance() == callback->GetInstance();
          });

      if (it == vec.end()) {
        map_[code].push_back(std::move(callback));
      }
    } else {
      map_[code].push_back(std::move(callback));
    }
  }

  void Unregister(EventCode code, const std::string &callback_id,
                  void *instance) {
    auto entry = map_.find(code);
    if (entry == map_.end()) {
      return;
    }

    auto &vec = entry->second;

    // remove_if only puts elements at the end
    // TODO: assert there's only a single element as a test
    // -> if yes, replace by a function that stops after 1st occurence
    auto it = std::remove_if(
        vec.begin(), vec.end(),
        [&callback_id, &instance](
            const std::unique_ptr<EventCallbackWrapperInterface> &elem) {
          // return elem->GetID() == callback_id;
          return elem->GetID() == callback_id &&
                 elem->GetInstance() == instance;
        });
    vec.erase(it, vec.end()); // actually delete
  }

  // rvalue since no need to keep the event?
  void DispatchEvent(Event &&e) {
    auto entry = map_.find(e.GetEvtCode());
    if (entry == map_.end()) {
      // HW_CORE_INFO("No callback entry found for this event. Dropping...");
      return;
    }

    auto &vec = entry->second;
    for (const auto &elem : vec) {
      elem->Trigger(e);
    }
  }

private:
  // static inline EventSystem* s_instance = nullptr;
  static inline std::unique_ptr<EventSystem> s_instance = nullptr;
  EventSystem() {
    map_ = std::unordered_map<
        EventCode,
        std::vector<std::unique_ptr<EventCallbackWrapperInterface>>>{};
  }

  std::unordered_map<
      EventCode, std::vector<std::unique_ptr<EventCallbackWrapperInterface>>>
      map_;
};

} // namespace weather
