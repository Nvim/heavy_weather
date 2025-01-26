#include "GLIndexBuffer.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"

#include <glad/glad.h>

namespace weather::graphics {

GLIndexBuffer::GLIndexBuffer(BufferDescriptor desc, void *indices)
    : Buffer{desc}, handle_{} {

  glGenBuffers(1, &handle_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->GetSize(), indices,
               GL_STATIC_DRAW);

  HW_CORE_TRACE("Created EBO #{}", handle_);
}

GLIndexBuffer::~GLIndexBuffer() {
  HW_CORE_TRACE("Destroying Index Buffer #{}", handle_);
  glDeleteBuffers(1, &handle_);
}

} // namespace weather::graphics
