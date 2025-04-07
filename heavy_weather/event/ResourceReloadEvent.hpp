#pragma once

/*
 * Notifies that a Resource (disk data, or GPU) has been reloaded.
 * */

#include "EventSystem.hpp"
#include "heavy_weather/event/Event.hpp"

namespace weather {

enum class ResourceType : u8 {
  IMAGE,
  SHADER_SOURCE,
  SHADER,
  SHADER_PROGRAM,
  MATERIAL,
  UNKNOWN,
};

class Image;
class ShaderSource;
namespace graphics {
class Material;
class Shader;
class ShaderProgram;
} // namespace graphics
using std::is_same_v;
template <typename T> class ResourceReloadEvent : public Event {
public:
  ResourceReloadEvent(T *e) : resource_{e} {
    // clang-format off
    if constexpr      (is_same_v<T, Image>)                   {type_ = ResourceType::IMAGE;}
    else if constexpr (is_same_v<T, ShaderSource>)            {type_ = ResourceType::SHADER_SOURCE;}
    else if constexpr (is_same_v<T, graphics::Shader>)        {type_ = ResourceType::SHADER;}
    else if constexpr (is_same_v<T, graphics::ShaderProgram>) {type_ = ResourceType::SHADER_PROGRAM;}
    else if constexpr (is_same_v<T, graphics::Material>)      {type_ = ResourceType::MATERIAL;}
    // clang-format on
    else {
      type_ = ResourceType::UNKNOWN;
      HW_CORE_ERROR("Couldn't set uniform value, bad type");
      HW_ASSERT(false);
    }
  }

  EventCode GetEvtCode() const override { return kCode; }
  static constexpr EventCode kCode = EventCode::EVENT_RESOURCE_RELOAD;

  T *GetResource() const { return resource_; }
  ResourceType GetType() const { return type_; };

private:
  T *resource_;
  ResourceType type_;
};

} // namespace weather
