#include "./GLFWInputManager.hpp"
#include <GLFW/glfw3.h>

namespace weather {
using std::pair;

GLFWInputManager::GLFWInputManager(void *window)
    : window_(static_cast<GLFWwindow *>(window)) {}

bool GLFWInputManager::IsKeyDown(int key) {
  i32 status = glfwGetKey(window_, key);
  return status == GLFW_PRESS || status == GLFW_REPEAT;
}

pair<f64, f64> GLFWInputManager::GetMousePos() {
  f64 x{}, y{};
  glfwGetCursorPos(window_, &x, &y);
  return {x, y};
}
} // namespace weather
