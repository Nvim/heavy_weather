#include "GLVertexBuffer.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"

#include <glad/glad.h>

namespace weather::graphics {

GLVertexBuffer::GLVertexBuffer(BufferDescriptor desc, void *vertices, u32 vao)
    : Buffer{desc}, handle_{}, vao_{vao}, layout_(*desc.layout) {

  // Create & bind buffers:
  glGenBuffers(1, &handle_);
  glBindBuffer(GL_ARRAY_BUFFER, handle_);

  // Fill buffer and configure vertex attributes:
  u16 i = 0;
  u64 offset = 0;
  auto stride = layout_.Stride();

  glBufferData(GL_ARRAY_BUFFER, this->Size(), vertices, GL_STATIC_DRAW);
  for (auto pair = layout_.Begin(), end = layout_.End(); pair != end; ++pair) {
    DataFormat format = pair->second;
    auto count = FormatCount(format);
    auto gl_type = FormatTOGL(format);
    glVertexAttribPointer(i, count, gl_type, GL_FALSE, stride,
                          (void *)(offset * sizeof(f32))); // NOLINT
    // HW_CORE_DEBUG("glVertexAttribPointer({}, {}, {}, GL_FALSE, {},
    // (void*){})", i, count, gl_type, stride, offset);
    glEnableVertexAttribArray(i);
    offset += count;
    ++i;
  }

  HW_CORE_TRACE("Created VBO #{}: VAO: {}. Count: {}. Size: {}", handle_, vao_,
                this->Count(), this->Size());
}

void GLVertexBuffer::SetLayout(VertexLayout &&layout) {
  this->layout_ = std::move(layout);
}

GLVertexBuffer::~GLVertexBuffer() {
  HW_CORE_TRACE("Destroying Vertex Buffer #{}, and VAO #{}", handle_, vao_);
  glDeleteBuffers(1, &handle_);
  glDeleteVertexArrays(1, &vao_);
}

} // namespace weather::graphics
