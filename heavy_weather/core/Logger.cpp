#include "Logger.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace weather {

std::shared_ptr<spdlog::logger> &Logger::GetAppLogger() { //
  return s_app_logger;
}

std::shared_ptr<spdlog::logger> &Logger::GetEngineLogger() {
  return s_engine_logger;
}

void Logger::Init() {
  spdlog::set_level(HW_LOG_LEVEL);
  auto app = spdlog::stdout_color_mt("APP");
  auto engine = spdlog::stdout_color_mt("CORE");

  app->set_pattern("[%M:%S:%e][%n][%^%l%$]: %v");
  engine->set_pattern("[%M:%S:%e][%n][%^%l%$]: %v");

  s_app_logger = app;
  s_engine_logger = engine;
}
} // namespace weather

/* Defining it here since it's the only asserts function */
void report_assertion_failure(const char *expression, const char *message,
                              const char *file, int line) {
  HW_CORE_CRITICAL(
      "Assertion Failure: {}, message: '{}', in file: {}, line: {:d}\n",
      expression, message, file, line);
}
