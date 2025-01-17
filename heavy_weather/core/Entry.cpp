#include "Entry.hpp"
#include "heavy_weather/core/Asserts.hpp"
#include "heavy_weather/core/Logger.hpp"

int main() {
  weather::Logger::Init();
  HW_APP_DEBUG("Engine starting in debug mode")

  weather::Application *app = weather::CreateAppHook();
  HW_ASSERT_MSG((app != nullptr), "Couldn't get handle to client app");

  app->Run();

  delete app; // NOLINT

  HW_APP_DEBUG("Engine shutting down");
  return 0;
}
