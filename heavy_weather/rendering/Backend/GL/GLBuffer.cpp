#include "GLBuffer.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/rendering/Backend/GL/GLAPI.hpp"
#include "heavy_weather/rendering/Backend/GL/Utils.hpp"
#include "heavy_weather/rendering/Types.hpp"

namespace weather::graphics {

GLBuffer::GLBuffer(BufferDescriptor &desc, void *data)
    : Buffer(desc), handle_{}, target_{GetGLBufferType(desc.type)} {
  GLint usage{};
  if (desc.type == BufferType::UniformBuffer) {
    usage = GL_DYNAMIC_DRAW;
  } else {
    usage = GL_STATIC_DRAW;
  }
  glGenBuffers(1, &handle_);
  glBindBuffer(target_, handle_);
  glBufferData(target_, this->Size(), data, usage);
}

GLBuffer::~GLBuffer() { glDeleteBuffers(1, &handle_); }

GLTarget GLBuffer::GLTarget(GLint t) {
  switch (t) {
  case GL_ARRAY_BUFFER:
    return GLTarget::VBO;
  case GL_ELEMENT_ARRAY_BUFFER:
    return GLTarget::EBO;
  case GL_UNIFORM_BUFFER:
    return GLTarget::UBO;
  default:
    HW_ASSERT_MSG(false, "Bad buffer type");
    return GLTarget::VBO;
  }
}

} // namespace weather::graphics
