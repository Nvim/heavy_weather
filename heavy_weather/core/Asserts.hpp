/*
 * Runtime asserts are automatically disabled by macro `NDEBUG`, which is
 * defined by CMake when `CMAKE_BUILD_TYPE` is release.
 * Static assertions stay in release.
 */

#pragma once

#ifdef HW_ENABLE_ASSERTS

// TODO: make more portable
#define debugBreak()                                                           \
  { __asm__ volatile("int $0x03"); }

/**
 * @brief Reports an assertion failure.
 * @param expression The expression to be reported.
 * @param message A custom message to be reported, if provided.
 * @param file The path and name of the file containing the expression.
 * @param line The line number in the file where the assertion failure occurred.
 */
void ReportAssertionFailure(const char *expression, const char *message,
                            const char *file, int line);

#define HW_STATIC_ASSERT(expr, msg)

#define HW_ASSERT(expr)                                                        \
  {                                                                            \
    if (expr) {                                                                \
    } else {                                                                   \
      ReportAssertionFailure(#expr, "", __FILE__, __LINE__);                   \
      debugBreak()                                                             \
    }                                                                          \
  }

#define HW_ASSERT_MSG(expr, msg)                                               \
  {                                                                            \
    if (expr) {                                                                \
    } else {                                                                   \
      ReportAssertionFailure(#expr, msg, __FILE__, __LINE__);                  \
      debugBreak()                                                             \
    }                                                                          \
  }

#else

#define HW_STATIC_ASSERT(expr, msg)
#define HW_ASSERT(expr)
#define HW_ASSERT_MSG(expr, msg)

#endif
