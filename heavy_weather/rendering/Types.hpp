#pragma once

#include "heavy_weather/engine.h"
#include <heavy_weather/core/Logger.hpp>
#include <utility>

namespace weather::graphics {

enum class Backend : u8 {
  OpenGL,
};

struct RendererInitParams {
  Backend backend;
  std::pair<u16, u16> viewport;
  bool depth_test;
  bool debug_mode;
};

struct ShaderDescriptor;   // Type, path, uniforms/attribs
struct BufferDescriptor;   // Type, size, layout
struct PipelineDescriptor; // Vertex and fragment shader to use
class VertexLayout;        // Vector of attributes (name, format, size)
class Buffer;
class Shader;
class ShaderProgram;

enum class DataFormat : u8 {
  Int,
  Int2,
  Int3,
  Int4,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
};

enum class BufferType : u8 {
  VertexBuffer,  /* Vertex attributes */
  IndexBuffer,   /* Vertex array indices */
  UniformBuffer, /* Uniform block storage */
  // TODO: Support these someday
  // GL_ATOMIC_COUNTER_BUFFER,     /* Atomic counter storage */
  // GL_COPY_READ_BUFFER,          /* Buffer copy source */
  // GL_COPY_WRITE_BUFFER,         /* Buffer copy destination */
  // GL_DISPATCH_INDIRECT_BUFFER,  /* Indirect compute dispatch commands */
  // GL_DRAW_INDIRECT_BUFFER,      /* Indirect command arguments */
  // GL_PIXEL_PACK_BUFFER,         /* Pixel read target */
  // GL_PIXEL_UNPACK_BUFFER,       /* Texture data source */
  // GL_QUERY_BUFFER,              /* Query result buffer */
  // GL_SHADER_STORAGE_BUFFER,     /* Read-write storage for shaders */
  // GL_TEXTURE_BUFFER,            /* Texture data buffer */
  // GL_TRANSFORM_FEEDBACK_BUFFER, /* Transform feedback buffer */
  Unknown,
};

struct BufferDescriptor {
  u64 size{0};
  u64 count{0};
  i32 binding = 0;
  BufferType type{BufferType::Unknown};
  VertexLayout *layout = nullptr;
};

enum class ShaderType : u8 { VertexShader, FragmentShader };

struct ShaderDescriptor {
  ShaderType type;
  std::string path;
};

struct PipelineDescriptor {
  UniquePtr<Shader> VertexShader;
  UniquePtr<Shader> FragmentShader;
};

enum ShaderCompileStatus : u8 { NotCompiled, Success, Failed };

struct MeshDescriptor {
  // void *vertices;
  std::pair<void *, u64> vertices;
  std::pair<u32 *, u64> indices;
  VertexLayout *layout;
  const char *name;
};

struct UniformDescriptor {
  const char *name;
  DataFormat format;
  void *data;
};

static inline u8 FormatCount(DataFormat format) {
  // clang-format off
  switch (format) {
    case DataFormat::Int:    return 1;
    case DataFormat::Int2:   return 2;
    case DataFormat::Int3:   return 3;
    case DataFormat::Int4:   return 4;
    case DataFormat::Float:  return 1;
    case DataFormat::Float2: return 2;
    case DataFormat::Float3: return 3;
    case DataFormat::Float4: return 4;
    case DataFormat::Mat3:   return 3*3;
    case DataFormat::Mat4:   return 4*4;
  }
  // clang-format on
  HW_CORE_WARN("Unkown buffer format.");
  return 0;
}

static inline u8 FormatSize(DataFormat format) {
  // clang-format off
  switch (format) {
    case DataFormat::Int:    return 1*4;
    case DataFormat::Int2:   return 2*4;
    case DataFormat::Int3:   return 3*4;
    case DataFormat::Int4:   return 4*4;
    case DataFormat::Float:  return 1*4;
    case DataFormat::Float2: return 2*4;
    case DataFormat::Float3: return 3*4;
    case DataFormat::Float4: return 4*4;
    case DataFormat::Mat3:   return 3*3*4;
    case DataFormat::Mat4:   return 4*4*4;
  }
  // clang-format on
  HW_CORE_WARN("Unkown buffer format.");
  return 0;
}

} // namespace weather::graphics
