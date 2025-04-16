#pragma once

#include "heavy_weather/rendering/Types.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace weather::graphics {

struct GuiDesc {
  Backend backend;
  void *window;
};

// Data from the base app class, displayed on every example app
struct AppInfo {
  const char *program_name;
  const char *engine_name;
  f64 frametime; // Time the frame actually took
  f64 fps;       // FPS, counting sleep to meet configured cap
};

class Gui {
public:
  static void Init(GuiDesc desc);
  static void ShutDown();
  // TODO: these should be private and App should be friend
  static void RenderAppWindow(AppInfo &info, void *window);
  static void BeginFrame();
  static void EndFrame();

  static bool BeginWindow(const char *title);
  static void EndWindow();

  static bool BeginTreeNode(const char *title);
  static bool BeginTreeNode();
  static void EndTreeNode();

  static bool DrawButton(const char *name);
  static bool DrawSliderFloat(const char *name, void *data, f32 min, f32 max);
  static bool DrawSliderFloat2(const char *name, void *data, f32 min, f32 max);
  static bool DrawSliderFloat3(const char *name, void *data, f32 min, f32 max);
  static bool DrawSliderFloat4(const char *name, void *data, f32 min, f32 max);
  static bool DrawColorEdit4(const char *name, void *data);
  static bool DrawInputFloat(const char *name, void *data);
  static bool DrawInputFloat2(const char *name, void *data);
  static bool DrawInputFloat3(const char *name, void *data);
  static bool DrawInputFloat4(const char *name, void *data);
  static bool DrawScalarInt(const char *name, void *data, i32 step);
  static bool DrawScalarFloat(const char *name, void *data, f32 step);
};

} // namespace weather::graphics
