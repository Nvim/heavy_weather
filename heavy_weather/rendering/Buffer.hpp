
#pragma once

#include "Types.hpp"

namespace weather::graphics {

class Buffer {
public:
  Buffer(BufferDescriptor desc)
      : type_{desc.type}, size_{desc.size}, count_{desc.count} {}
  virtual ~Buffer() = default;

  u32 Size() const { return size_; }
  u32 Count() const { return count_; }
  // i32 Binding() const { return binding_; }
  // virtual const VertexLayout *Layout() const = 0; // can return null
  // virtual void SetLayout(VertexLayout &&layout) = 0;
  BufferType Type() const { return type_; }

  //
  Buffer(const Buffer &) = default;
  Buffer(Buffer &&) = delete;
  Buffer &operator=(const Buffer &) = delete;
  Buffer &operator=(Buffer &&) = delete;

private:
  BufferType type_;
  u64 size_;  // Size of the buffer in bytes
  u64 count_; // Element count of the buffer
};

} // namespace weather::graphics
