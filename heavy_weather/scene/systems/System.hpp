#pragma once

#include "heavy_weather/ecs/ecs.hpp"
#include <functional>

namespace weather {

using System = std::function<void(ECS &, f64)>;

void RenderSystem(ECS &ecs, f64 delta);
void RotateSystem(ECS &ecs, f64 delta);

} // namespace weather
