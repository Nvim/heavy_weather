// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on
#include "LinuxWindow.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Input/InputManager.hpp"
#include "heavy_weather/core/Logger.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/EventSystem.hpp"
#include "heavy_weather/event/KeyPressedEvent.hpp"
#include "heavy_weather/event/MouseMovedEvent.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/platform/GLFWInputManager.hpp"
#include "heavy_weather/rendering/Types.hpp"
#include <chrono>
#include <thread>
#include <unistd.h>

namespace weather {
namespace {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
bool s_glfw_init = false;

void ErrorCallback(int error, const char *description) {
  HW_CORE_ERROR("GLFW Error: {}: {}", error, description);
}

} // namespace

// ** Platform functions ** //
void PlatformSleep(u64 time) {
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

f64 PlatformGetTime() {
  struct timespec now {};
  clock_gettime(CLOCK_MONOTONIC, &now);
  f64 time = now.tv_sec + now.tv_nsec * 0.000000001; // NOLINT
  if (s_glfw_init) {
    glfwSetTime(time);
  }
  return time;
}

std::unique_ptr<Window> PlatformInitWindow(const WindowProps &props) {
  return std::make_unique<LinuxWindow>(props);
}

InputManager *PlatformInitInput(void *window) {
  // TODO shared_ptr
  return new GLFWInputManager(window); // NOLINT
}

// TODO: gracefully handle errors
bool PlatformLoadBackend(graphics::Backend backend) {
  HW_ASSERT_MSG(backend == graphics::Backend::OpenGL,
                "Backend is not supported on Linux");
  // NOLINTNEXTLINE
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    HW_ASSERT_MSG(0, "Couldn't load OpenGL");
  }
  return true;
}

// TODO: this takes a backend as param:
static void set_window_flags() {
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef GL_VERSION_4
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#else
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif // GL_VERSION_4
#ifdef HW_DEBUG
  HW_CORE_DEBUG("Set debug mode window hint");
  glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif // HW_DEBUG
}

// ** LinuxWindow class ** //
LinuxWindow::LinuxWindow(const WindowProps &props) : props_{props} {
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND); // TODO: define for this
  HW_ASSERT_MSG((glfwInit()), "Couldn't init GLFW");
  set_window_flags();
  window_ = glfwCreateWindow(props.width, props.height, props.title.c_str(),
                             nullptr, nullptr);
  if (!window_) {
    HW_CORE_CRITICAL("Couldn't create GLFW window");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window_);
  glfwSetWindowUserPointer(window_, &props_);

  glfwSwapInterval(0);

  glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {
    // clang-format off
      (void)mods; (void)action; (void)window; (void)scancode;
    // clang-format on
    EventSystem::Get().DispatchEvent(KeyPressedEvent{key, action});
  });

  glfwSetWindowSizeCallback(
      window_, [](GLFWwindow *window, int width, int height) {
        auto &p = *static_cast<WindowProps *>(glfwGetWindowUserPointer(window));
        EventDispatch(ResizeEvent{static_cast<u16>(width),
                                  static_cast<u16>(height), p.width, p.height});
        p.width = width;
        p.height = height;
      });

  glfwSetErrorCallback(ErrorCallback);
  glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
    EventDispatch(WindowCloseEvent{window});
  });
  glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double x, double y) {
    (void)window;
    EventDispatch(MouseMovedEvent{x, y});
  });

  s_glfw_init = true;
  HW_CORE_DEBUG("Window created");
}

void LinuxWindow::Close() { glfwSetWindowShouldClose(window_, GLFW_TRUE); }

LinuxWindow::~LinuxWindow() { glfwDestroyWindow(window_); }

void LinuxWindow::Update() {
  glfwSwapBuffers(window_);
  glfwPollEvents();
}

void *LinuxWindow::GetNative() { return window_; }

const WindowProps &LinuxWindow::GetProps() const { return props_; }

} // namespace weather
