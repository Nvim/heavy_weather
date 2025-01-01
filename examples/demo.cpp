#include "heavy_weather/core/Application.hpp"
#include "heavy_weather/core/Entry.hpp"

// int main() {
//   weather::Logger::Init();
//   weather::Application app{};
//
//   app.Run();
//
//   return 0;
// }

class Demo : public weather::Application {
public:
  Demo() {}
  ~Demo() {}
};

weather::Application *weather::CreateAppHook() { return new Demo{}; }
