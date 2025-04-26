#include "GLIndexBuffer.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/VertexLayout.hpp"

#include <glad/glad.h>

namespace weather::graphics {

GLIndexBuffer::GLIndexBuffer(BufferDescriptor desc, void *indices)
    : Buffer{desc}, handle_{} {

  glGenBuffers(1, &handle_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Size(), indices, GL_STATIC_DRAW);

  HW_CORE_TRACE("Created EBO #{}", handle_);
}

GLIndexBuffer::~GLIndexBuffer() {
  HW_CORE_TRACE("Destroying Index Buffer #{}", handle_);
  glDeleteBuffers(1, &handle_);
}

// const VertexLayout *GLIndexBuffer::Layout() const {
//   HW_CORE_ERROR("Trying to get layout on index buffer is invalid");
//   return nullptr;
// }
//
// void GLIndexBuffer::SetLayout(VertexLayout &&layout) {
//   (void)layout;
//   HW_CORE_ERROR("Can't set layout on index buffer");
// }

} // namespace weather::graphics
