/*
 * Utility functions for working with OpenGL
 * */

#pragma once

#include <glad/glad.h>
#include <heavy_weather/engine.h>
#include <heavy_weather/rendering/Types.hpp>

namespace weather::graphics {

static inline u32 FormatTOGL(DataFormat format) {
  // clang-format off
  switch (format) {
    case DataFormat::Int:
    case DataFormat::Int2:
    case DataFormat::Int3:
    case DataFormat::Int4:
      return GL_INT;
    case DataFormat::Float:
    case DataFormat::Float2:
    case DataFormat::Float3:
    case DataFormat::Float4:
    case DataFormat::Mat3:
    case DataFormat::Mat4:
      return GL_FLOAT;
  }
  // clang-format on
  HW_CORE_WARN("Unkown buffer format.");
  return 0;
}

void APIENTRY LogDebugMessage(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *userParam);

} // namespace weather::graphics
