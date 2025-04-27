#include "GLRenderTarget.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/rendering/Backend/GL/GLBuffer.hpp"

namespace weather::graphics {

GLRenderTarget::GLRenderTarget(std::pair<i32, i32> size)
    : size_{size}, handle_{}, color_{}, depth_{} {

  glGenFramebuffers(1, &handle_);
  glBindFramebuffer(GL_FRAMEBUFFER, handle_);

  // Color:
  glGenTextures(1, &color_);
  glBindTexture(GL_TEXTURE_2D, color_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.first, size.second, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Depth:
  glGenTextures(1, &depth_);
  glBindTexture(GL_TEXTURE_2D, depth_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.first, size.second, 0,
               GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach:
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         color_, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         depth_, 0);

  HW_ASSERT_MSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
                    GL_FRAMEBUFFER_COMPLETE,
                "failed to create OpenGL FBO");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLRenderTarget::~GLRenderTarget() { glDeleteFramebuffers(0, &handle_); }
} // namespace weather::graphics
