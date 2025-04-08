#pragma once

#include <heavy_weather/engine.h>

// Position only
static inline f32 cube_verts[] = {
    // clang-format off
  -0.5f, -0.5f, 0.5f,
  -0.5f,  0.5f, 0.5f,
   0.5f, -0.5f, 0.5f,
   0.5f,  0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f,  0.5f, -0.5f,
   0.5f, -0.5f, -0.5f,
   0.5f,  0.5f, -0.5f,
    // clang-format on
};

static inline u32 cube_indices[] = {
    // clang-format off
  0, 2, 3,
  0, 3, 1,
  2, 6, 7,
  2, 7, 3,
  6, 4, 5,
  6, 5, 7,
  4, 0, 1,
  4, 1, 5,
  0, 4, 6,
  0, 6, 2,
  1, 5, 7,
  1, 7, 3,
    // clang-format on
};

static inline f32 cube_again[] = {
    // clang-format off
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    // clang-format on
};

// Position, Normal, and UV
static inline f32 cube_verts_normals_uvs[] = {
    // clang-format off
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top-right
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Top-left

    // Back face
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0, 0.0f, 0.0f, // Bottom-left
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0, 1.0f, 0.0f, // Bottom-right
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0, 1.0f, 1.0f, // Top-right
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0, 0.0f, 1.0f, // Top-left

    // Left face
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0, 0.0f, 0.0f, // Bottom-left
    0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0, 1.0f, 0.0f, // Bottom-right
    0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0, 1.0f, 1.0f, // Top-right
    0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0, 0.0f, 1.0f, // Top-left

    // Right face
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-right
    1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left

    // Top face
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Top-right
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top-left

    // Bottom face
    1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 0.0f, 0.0f, // Bottom-left
    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 1.0f, 0.0f, // Bottom-right
    0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0, 1.0f, 1.0f, // Top-right
    1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0, 0.0f, 1.0f, // Top-left
    // clang-format on
};

static inline u32 cube_indices_big[] = {
    // clang-format off
    // Front face (2 triangles)
    0, 1, 2, // First triangle (bottom-left, bottom-right, top-right)
    0, 2, 3, // Second triangle (bottom-left, top-right, top-left)

    // Back face (2 triangles)
    4, 5, 6, // First triangle (bottom-left, bottom-right, top-right)
    4, 6, 7, // Second triangle (bottom-left, top-right, top-left)

    // Left face (2 triangles)
    8, 9, 10, // First triangle (bottom-left, bottom-right, top-right)
    8, 10, 11, // Second triangle (bottom-left, top-right, top-left)

    // Right face (2 triangles)
    12, 13, 14, // First triangle (bottom-left, bottom-right, top-right)
    12, 14, 15, // Second triangle (bottom-left, top-right, top-left)

    // Top face (2 triangles)
    16, 17, 18, // First triangle (bottom-left, bottom-right, top-right)
    16, 18, 19, // Second triangle (bottom-left, top-right, top-left)

    // Bottom face (2 triangles)
    20, 21, 22, // First triangle (bottom-left, bottom-right, top-right)
    20, 22, 23, // Second triangle (bottom-left, top-right, top-left)
    // clang-format on
};
