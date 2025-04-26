#include "System.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/platform/Platform.hpp"
#include "heavy_weather/rendering/TransformComponent.hpp"
#include "heavy_weather/scene/components/RotateComponent.hpp"
#include <cmath>
#include <glm/trigonometric.hpp>

namespace weather {

void RotateSystem(ECS &ecs, f64 delta) {
  auto entities = ecs.Query<graphics::TransformComponent, RotateComponent>();
  for (auto e : entities) {
    auto *tr = ecs.GetComponentPtr<graphics::TransformComponent>(e);
    auto rot = ecs.GetComponent<RotateComponent>(e);

    f32 *rots[3] = {&tr->rotation.x, &tr->rotation.y, &tr->rotation.z};
    f32 speeds[3] = {rot.x_speed, rot.y_speed, rot.z_speed};
    auto s = delta / 100;

    for (u8 i = 0; i < 3; ++i) {
      *(rots[i]) += speeds[i] * s;
      if (*(rots[i]) > 360.0f) {
        *(rots[i]) = 0.0f;
      }
    }

    tr->dirty = true;
  }
}

} // namespace weather
