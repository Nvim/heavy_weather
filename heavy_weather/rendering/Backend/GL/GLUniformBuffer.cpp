#include "GLUniformBuffer.hpp"

#include <glad/glad.h>

namespace weather::graphics {

GLUniformBuffer::GLUniformBuffer(BufferDescriptor desc, void *data)
    : Buffer{desc}, handle_{} {
  (void)data;
  glGenBuffers(1, &handle_);
  glBindBuffer(GL_UNIFORM_BUFFER, handle_);
  glBufferData(GL_UNIFORM_BUFFER, Size(), nullptr, GL_DYNAMIC_DRAW);
  // glBindBufferBase(GL_UNIFORM_BUFFER, Binding(), handle_);
  HW_CORE_TRACE("Created UBO #{}", handle_);
}

GLUniformBuffer::~GLUniformBuffer() {
  HW_CORE_TRACE("Destroying Uniform Buffer #{}", handle_);
  glDeleteBuffers(1, &handle_);
}

void GLUniformBuffer::SetLayout(VertexLayout &&layout) {
  this->layout_ = std::move(layout);
}

} // namespace weather::graphics
