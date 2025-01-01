#include "heavy_weather/core/Application.hpp"
#include "heavy_weather/core/Asserts.hpp"


int main() {
  weather::Logger::Init();
  weather::Application app{};
#ifdef HW_DEBUG
  HW_APP_DEBUG("HW_DEBUG is defined")
#endif

#ifdef NDEBUG
  HW_APP_INFO("Non debug")
#endif

  HW_APP_TRACE("Trace")
  HW_APP_DEBUG("Debug")
  HW_APP_INFO("Info")
  HW_APP_WARN("Warn")
  HW_APP_ERROR("Error")
  HW_APP_CRITICAL("Critical")

  app.Run();

  return 0;
}
