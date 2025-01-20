#include "GLVertexBuffer.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"

#include <glad/glad.h>

namespace weather::graphics {

GLVertexBuffer::GLVertexBuffer(BufferDescriptor desc, void *vertices)
    : Buffer{desc}, handle_{}, vao_{}, layout_(*desc.layout) {

  // Create & bind buffers:
  glGenBuffers(1, &vao_);
  glGenBuffers(1, &handle_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, handle_);

  // Fill buffer and configure vertex attributes:
  u16 i = 0;
  u64 offset = 0;
  auto stride = layout_.Stride();
  HW_ASSERT_MSG(this->GetSize() / 3 == stride, "Weird padding");

  glBufferData(GL_ARRAY_BUFFER, this->GetSize(), vertices, GL_STATIC_DRAW);
  for (auto pair = layout_.Begin(), end = layout_.End(); pair != end; ++pair) {
    DataFormat format = pair->second;
    auto count = FormatCount(format);
    auto gl_type = FormatTOGL(format);
    glVertexAttribPointer(i, count, gl_type, GL_FALSE, stride,
                          (void *)(offset * sizeof(float))); // NOLINT
    // HW_CORE_DEBUG("glVertexAttribPointer({}, {}, {}, GL_FALSE, {},
    // (void*){})", i, count, gl_type, stride, offset);
    glEnableVertexAttribArray(i);
    offset += count;
    ++i;
  }

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  HW_CORE_DEBUG("Created VBO #{} and VAO #{}", handle_, vao_);

  // Unbind:
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

GLVertexBuffer::~GLVertexBuffer() {
  HW_CORE_TRACE("Destroying Vertex Buffer #{}", handle_);
  glDeleteBuffers(1, &handle_);
}

} // namespace weather::graphics
