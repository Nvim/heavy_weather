#include "Utils.hpp"

#include <glad/glad.h>
#include <heavy_weather/engine.h>
#include <heavy_weather/rendering/Types.hpp>
#include <spdlog/fmt/fmt.h>

void APIENTRY weather::graphics::LogDebugMessage(
    GLenum source, GLenum type, unsigned int id, GLenum severity,
    GLsizei length, const char *message, const void *user_param) {
  // clang-format off
  // ignore non-significant error/warning codes
#ifndef GL_CONTEXT_FLAG_DEBUG_BIT
  (void)source; (void)type; (void)id; (void)severity; (void)length;
  (void)message; (void)user_param;
#else
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 
  (void)length;
  (void)user_param;

  std::string src, t, sev;
  switch (source)
  {
    case GL_DEBUG_SOURCE_API:             src = "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   src = "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     src = "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     src = "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           
    default:                              src = "Source: Other"; break;
  }

  switch (type)
  {
    case GL_DEBUG_TYPE_ERROR:               t = "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: t = "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  t = "Type: Undefined Behaviour"; break; 
    case GL_DEBUG_TYPE_PORTABILITY:         t = "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         t = "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              t = "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          t = "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           t = "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:              
    default:
                                            t = "Type: Other"; break;
  }

  switch (severity)
  {
    case GL_DEBUG_SEVERITY_HIGH:
      sev = "Error"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      sev = "Warning"; break;
    case GL_DEBUG_SEVERITY_LOW:
      sev = "Info"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: 
    default:
      sev = "Message";
  }
  // clang-format on
  std::string s =
      fmt::format("OPENGL {}(Code #{}): {}. {}. {}", sev, id, message, t, src);
  HW_CORE_INFO("{}", s)
#endif
}
