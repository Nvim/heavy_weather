#pragma once

#include <heavy_weather/engine.h>

static inline f32 cube_verts[] = {
    -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,  0.5f,  0.5f,  -0.5f,
    0.5f,  0.5f,  0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
    0.5f,  -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
};

static inline u32 cube_indices[] = {
    0, 2, 3, 0, 3, 1, 2, 6, 7, 2, 7, 3, 6, 4, 5, 6, 5, 7,
    4, 0, 1, 4, 1, 5, 0, 4, 6, 0, 6, 2, 1, 5, 7, 1, 7, 3,
};

// static constexpr inline float kCubeVerts[] = {};
