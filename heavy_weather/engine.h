// Global defines
#pragma once

#include <cstdint>
#include <memory>

#define HW_ENABLE_GUI 1

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

template <typename T> //
using UniquePtr = std::unique_ptr<T>;

template <typename T> //
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using WeakPtr = std::weak_ptr<T>;
