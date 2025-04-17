#include "System.hpp"
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

    auto s = sin(delta) * 10;
    tr->rotation.x += rot.x_speed * s;
    tr->rotation.y += rot.y_speed * s;
    tr->rotation.z += rot.z_speed * s;

    // tr->rotation.x = glm::clamp((f32)(tr->rotation.x + (rot.x_speed * s)),
    // 0.0f, 360.0f); tr->rotation.x = glm::clamp((f32)(tr->rotation.x +
    // (rot.x_speed * s)), 0.0f, 360.0f);
    tr->dirty = true;
  }
}

} // namespace weather
