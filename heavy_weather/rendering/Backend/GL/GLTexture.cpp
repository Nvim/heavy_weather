#include "GLTexture.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/event/EventSystem.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/rendering/Texture.hpp"
#include "heavy_weather/resources/Image.hpp"
#include <glad/glad.h>

static GLint WrapFlagToGL(weather::graphics::TextureWrapFlag flag);
static GLint FilterFlagToGl(weather::graphics::TextureFilterFlag flag);

namespace weather::graphics {
GLTexture::GLTexture(SharedPtr<Image> img, const TextureParams &params)
    : Texture{img} {

  GLint format = GL_RGB;
  if (img->Channels() == 4) {
    format = GL_RGBA;
  }

  glGenTextures(1, HandlePtr());
  glActiveTexture(GL_TEXTURE0 + Unit());
  SetParams(params); // Will bind the texture

  glTexImage2D(GL_TEXTURE_2D, 0, format, (i32)img->Size().first,
               (i32)img->Size().second, 0, format, GL_UNSIGNED_BYTE,
               img->Data());
  glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::Reload() {
  HW_CORE_DEBUG("Texture: reloading");
  GetImage().Reload();
}

void GLTexture::OnResourceReload(const ResourceReloadEvent<Image> &evt) {
  if (evt.GetType() != ResourceType::IMAGE ||
      evt.GetResource()->Path() != Path()) {
    return;
  }
  HW_CORE_DEBUG("Texture: Handling ResourceReloadEvent<Image>");
  GLint format = GL_RGB;
  Image &img = GetImage();
  if (img.Channels() == 4) {
    format = GL_RGBA;
  }
  //
  Bind();
  glTexImage2D(GL_TEXTURE_2D, 0, format, (i32)img.Size().first,
               (i32)img.Size().second, 0, format, GL_UNSIGNED_BYTE, img.Data());
  glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::Bind() const {
  glActiveTexture(GL_TEXTURE0 + Unit());
  glBindTexture(GL_TEXTURE_2D, Handle());
}

GLTexture::~GLTexture() { glDeleteTextures(1, HandlePtr()); }

void GLTexture::SetParams(const TextureParams &params) {
  glBindTexture(GL_TEXTURE_2D, Handle());
  SetMinFilterFlag(params.MinFilter);
  SetMagFilterFlag(params.MagFilter);
  SetWrapFlag(params.Wrap);
}

void GLTexture::SetMinFilterFlag(TextureFilterFlag flag) {
  auto value = FilterFlagToGl(flag);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, value);
}

void GLTexture::SetMagFilterFlag(TextureFilterFlag flag) {
  auto value = FilterFlagToGl(flag);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, value);
}

void GLTexture::SetWrapFlag(TextureWrapFlag flag) {
  auto value = WrapFlagToGL(flag);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, value);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, value);
}
} // namespace weather::graphics

static GLint FilterFlagToGl(weather::graphics::TextureFilterFlag flag) {
  switch (flag) {
  case weather::graphics::TextureFilterFlag::LINEAR:
    return GL_LINEAR;
  case weather::graphics::TextureFilterFlag::NEAREST:
    return GL_NEAREST;
  case weather::graphics::TextureFilterFlag::MIPMAP_NEAREST_NEAREST:
    return GL_NEAREST_MIPMAP_NEAREST;
  case weather::graphics::TextureFilterFlag::MIPMAP_NEAREST_LINEAR:
    return GL_NEAREST_MIPMAP_LINEAR;
  case weather::graphics::TextureFilterFlag::MIPMAP_LINEAR_LINEAR:
    return GL_LINEAR_MIPMAP_LINEAR;
  case weather::graphics::TextureFilterFlag::MIPMAP_LINEAR_NEAREST:
  default:
    return GL_LINEAR_MIPMAP_NEAREST;
  }
}

static GLint WrapFlagToGL(weather::graphics::TextureWrapFlag flag) {
  switch (flag) {
  case weather::graphics::TextureWrapFlag::REPEAT_MIRROR:
    return GL_MIRRORED_REPEAT;
  case weather::graphics::TextureWrapFlag::CLAMP_EDGE:
    return GL_CLAMP_TO_EDGE;
  case weather::graphics::TextureWrapFlag::CLAMP_BORDER:
    return GL_CLAMP_TO_BORDER;
  case weather::graphics::TextureWrapFlag::REPEAT:
  default:
    return GL_REPEAT;
  }
}
