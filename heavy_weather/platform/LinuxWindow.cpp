// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
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

namespace weather {
namespace {
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  (void)mods;
  (void)action;
  (void)window;
  (void)scancode;
  // Moving stack memory?
  KeyPressedEvent e{key, action};
  EventSystem::get().DispatchEvent(std::move(e));
}

void resize_callback(GLFWwindow *window, int width, int height) {
  s_WindowProps &props = *(s_WindowProps *)(glfwGetWindowUserPointer(window));
  EventDispatch(ResizeEvent{props.width, props.height, static_cast<u16>(width),
                            static_cast<u16>(height)});
}

// TODO: log this
void error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

} // namespace

LinuxWindow::LinuxWindow(const s_WindowProps &props) : props_{props} {

  // TODO: Do this from OpenGL 'platform init'
  glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    HW_CORE_ERROR("Couldn't load OpenGL");
    glfwDestroyWindow(window_);
    glfwTerminate();
    return;
  }
  glfwSwapInterval(1);

  // TODO: Use events for key, resize and close callback
  glfwSetKeyCallback(window_, key_callback);
  glfwSetWindowSizeCallback(window_, resize_callback);
  glfwSetErrorCallback(error_callback);
  glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
    // s_WindowProps &props = *(s_WindowProps
    // *)glfwGetWindowUserPointer(window);
    EventDispatch(WindowCloseEvent{window});
  });
  glfwSetCursorPosCallback(window_, [](GLFWwindow *window, double x, double y) {
    (void)window;
    EventDispatch(MouseMovedEvent{x, y});
  });

  HW_CORE_DEBUG("Window created");
}

void LinuxWindow::Close() { glfwSetWindowShouldClose(window_, GLFW_TRUE); }

LinuxWindow::~LinuxWindow() { glfwDestroyWindow(window_); }

void LinuxWindow::Update() {
  glfwSwapBuffers(window_);
  glfwPollEvents();
}

const s_WindowProps &LinuxWindow::GetProps() const { return props_; }

} // namespace weather
