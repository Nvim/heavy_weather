#pragma once

#include "heavy_weather/engine.h"
#include <heavy_weather/core/Logger.hpp>

namespace weather::graphics {

enum class Backend : u8 {
  OpenGL,
};

struct ShaderDescriptor;   // Type, path, uniforms/attribs
struct BufferDescriptor;   // Type, size, layout
struct PipelineDescriptor; // Vertex and fragment shader to use
class VertexLayout;        // Vector of attributes (name, format, size)
class Buffer;
class Shader;
class Pipeline;

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
  VertexBuffer,
  IndexBuffer,
};

/*
 * Shader = Returned by renderer->CreateShader(path, type) = compiled shader
 * Pipeline = Program made of two shader objects. renderer->CreatePipeline(vs,
 * fs) = Linked program To bind shaders, renderer->UsePipeline(pipeline)
 * */
struct BufferDescriptor {
  u64 size;
  u64 count;
  BufferType type;
  VertexLayout *layout;
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
