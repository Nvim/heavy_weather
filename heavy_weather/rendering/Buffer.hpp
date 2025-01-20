
#pragma once

#include "Types.hpp"
#include "VertexLayout.hpp"

namespace weather::graphics {

class Buffer {
public:
  Buffer(BufferDescriptor desc)
      : type_{desc.type}, size_{desc.size}, count_{desc.count} {}
  virtual ~Buffer() = default;

  u32 GetSize() const { return size_; }
  u32 GetCount() const { return count_; }
  BufferType Type() const { return type_; }

  //
  Buffer(const Buffer &) = default;
  Buffer(Buffer &&) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer &operator=(Buffer &&) = delete;

private:
  BufferType type_;
  u32 size_;  // Size of the buffer in bytes
  u32 count_; // Element count of the buffer
};

} // namespace weather::graphics
