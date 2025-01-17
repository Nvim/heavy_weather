#include "./GLFWInputManager.hpp"
#include <GLFW/glfw3.h>

namespace weather {

GLFWInputManager::GLFWInputManager(void *window) {
  window_ = static_cast<GLFWwindow *>(window);
}

bool GLFWInputManager::isKeyDown(int key) {
  i32 status = glfwGetKey(window_, key);
  if (status == GLFW_PRESS || status == GLFW_REPEAT) {
    return true;
  }
  return false;
}

std::pair<f64, f64> GLFWInputManager::getMousePos() {
  f64 x, y;
  glfwGetCursorPos(static_cast<GLFWwindow *>(window_), &x, &y);
  return std::pair<f64, f64>(x, y);
}
} // namespace weather
