#include "GLFWInputManager.hpp"
#include <GLFW/glfw3.h>

namespace weather {
bool GLFWInputManager::isKeyDown(int key) {
  i32 status = glfwGetKey(static_cast<GLFWwindow*>(s_window), key);
  if (status == GLFW_PRESS || status == GLFW_REPEAT) {
    return true;
  }
  return false;
}

std::pair<f64, f64> GLFWInputManager::getMousePos() {
  f64 x, y;
  glfwGetCursorPos(static_cast<GLFWwindow*>(s_window), &x, &y);
  return std::pair<f64, f64>(x, y);
}
} // namespace weather
