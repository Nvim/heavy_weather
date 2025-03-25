#include "GLTexture.hpp"
#include "heavy_weather/loaders/Image.hpp"
#include <filesystem>
#include <glad/glad.h>

namespace weather::graphics {
GLTexture::GLTexture(const std::string &path) : Texture{path} {
  std::filesystem::path p{path};
  Image img{p};
  if (img.Empty()) {
    return;
  }
  auto last_dot = path.find_last_of('.');
  auto last_slash = path.find_last_of('/');
  if (last_dot == std::string::npos) {
    last_dot = path.length();
  }
  if (last_slash == std::string::npos) {
    last_slash = 0;
  }
  SetName(path.substr(last_slash, last_dot));

  GLint format = GL_RGB;
  if (img.Channels() == 4) {
    format = GL_RGBA;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenTextures(1, HandlePtr());
  glActiveTexture(GL_TEXTURE0 + Unit());
  glBindTexture(GL_TEXTURE_2D, Handle());

  glTexImage2D(GL_TEXTURE_2D, 0, format, (i32)img.Size().first,
               (i32)img.Size().second, 0, format, GL_UNSIGNED_BYTE, img.Data());
  glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::Bind() const { glBindTexture(GL_TEXTURE_2D, Handle()); }

GLTexture::~GLTexture() { glDeleteTextures(1, HandlePtr()); }

// TODO:
void GLTexture::SetFilterFlag(TextureFilterFlag flag) { (void)flag; }
void GLTexture::SetWrapFlag(TextureWrapFlag flag) { (void)flag; }

} // namespace weather::graphics
