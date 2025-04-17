#include "System.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"
#include <cmath>

namespace weather {

void RenderSystem(ECS &ecs, f64 delta) {
  (void)ecs;
  (void)delta;
  // TODO: this shouldn't be a standard system. It's execution always happens at
  // a fixed moment, and it needs to use renderer.
}

} // namespace weather
