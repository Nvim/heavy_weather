// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "heavy_weather/core/Input/InputManager.hpp"
#include "heavy_weather/core/Logger.hpp"
// clang-format on
#include "LinuxWindow.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Window.hpp"
#include "heavy_weather/event/EventSystem.hpp"
#include "heavy_weather/event/KeyPressedEvent.hpp"
#include "heavy_weather/event/MouseMovedEvent.hpp"
#include "heavy_weather/event/ResizeEvent.hpp"
#include "heavy_weather/event/Util.hpp"
#include "heavy_weather/event/WindowCloseEvent.hpp"
#include "heavy_weather/platform/GLFWInputManager.hpp"
#include <GL/gl.h>
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

// ** LinuxWindow class ** //
LinuxWindow::LinuxWindow(const WindowProps &props) : props_{props} {

  // TODO: Do this from OpenGL 'platform init'
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  HW_ASSERT_MSG((glfwInit()), "Couldn't init GLFW");
  window_ = glfwCreateWindow(props.width, props.height, props.title.c_str(),
                             nullptr, nullptr);
  if (!window_) {
    HW_CORE_CRITICAL("Couldn't create GLFW window");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window_);
  glfwSetWindowUserPointer(window_, &props_);

  // NOLINTNEXTLINE
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    HW_CORE_ERROR("Couldn't load OpenGL");
    glfwDestroyWindow(window_);
    glfwTerminate();
    return;
  }
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
        EventDispatch(ResizeEvent{p.width, p.height, static_cast<u16>(width),
                                  static_cast<u16>(height)});
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
