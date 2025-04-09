/* *
 * Utility functions for working with OpenGL
 * */

#pragma once

#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
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

static inline void BindUniform1i(int loc, void *data) {
  auto d = *static_cast<i32 *>(data);
  glUniform1i(loc, d);
}

static inline void BindUniform1f(int loc, void *data) {
  auto d = *static_cast<float *>(data);
  glUniform1f(loc, d);
}

static inline void BindUniform2f(int loc, void *data) {
  auto d = *static_cast<glm::vec2 *>(data);
  glUniform2f(loc, d.x, d.y); // NOLINT
}

static inline void BindUniform3f(int loc, void *data) {
  auto d = *static_cast<glm::vec3 *>(data);
  glUniform3f(loc, d.x, d.y, d.z); // NOLINT
}

static inline void BindUniform4f(int loc, void *data) {
  auto d = *static_cast<glm::vec4 *>(data);
  glUniform4f(loc, d.x, d.y, d.z, d.w); // NOLINT
}

static inline void BindUniformMat3f(int loc, void *data) {
  auto d = *static_cast<glm::mat3x3 *>(data);
  glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(d));
}

static inline void BindUniformMat4f(int loc, void *data) {
  auto d = *static_cast<glm::mat4x4 *>(data);
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(d));
}

void APIENTRY LogDebugMessage(GLenum source, GLenum type, unsigned int id,
                              GLenum severity, GLsizei length,
                              const char *message, const void *user_param);

} // namespace weather::graphics
