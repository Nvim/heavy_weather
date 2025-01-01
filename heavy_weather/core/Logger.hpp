/*
 * Logger is a static class that wraps 2 spdlog loggers.
 * - One for engine's logs
 * - The other for client app's logs
 * For now, the two have identical settings:
 * Multithreaded, stdout, colored output. Basically defaults.
 * Convinience macros defined outside the namespace.
 * TODO: find a way to expose only the app logger macros
 */
#pragma once

#ifdef HW_DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define HW_LOG_LEVEL spdlog::level::level_enum::trace
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#define HW_LOG_LEVEL spdlog::level::level_enum::info
#endif

#include "spdlog/logger.h"
#include <memory>

#define HW_CORE_TRACE(...)                                                     \
  weather::Logger::GetEngineLogger()->trace(__VA_ARGS__);
#define HW_CORE_DEBUG(...)                                                     \
  weather::Logger::GetEngineLogger()->debug(__VA_ARGS__);
#define HW_CORE_INFO(...) weather::Logger::GetEngineLogger()->info(__VA_ARGS__);
#define HW_CORE_WARN(...) weather::Logger::GetEngineLogger()->warn(__VA_ARGS__);
#define HW_CORE_ERROR(...)                                                     \
  weather::Logger::GetEngineLogger()->error(__VA_ARGS__);
#define HW_CORE_CRITICAL(...)                                                  \
  weather::Logger::GetEngineLogger()->critical(__VA_ARGS__);

#define HW_APP_TRACE(...) weather::Logger::GetAppLogger()->trace(__VA_ARGS__);
#define HW_APP_DEBUG(...) weather::Logger::GetAppLogger()->debug(__VA_ARGS__);
#define HW_APP_INFO(...) weather::Logger::GetAppLogger()->info(__VA_ARGS__);
#define HW_APP_WARN(...) weather::Logger::GetAppLogger()->warn(__VA_ARGS__);
#define HW_APP_ERROR(...) weather::Logger::GetAppLogger()->error(__VA_ARGS__);
#define HW_APP_CRITICAL(...)                                                   \
  weather::Logger::GetAppLogger()->critical(__VA_ARGS__);

namespace weather {
class Logger {
public:
  static void Init();
  static std::shared_ptr<spdlog::logger> &GetAppLogger();
  static std::shared_ptr<spdlog::logger> &GetEngineLogger();

private:
  static inline std::shared_ptr<spdlog::logger> s_app_logger{};
  static inline std::shared_ptr<spdlog::logger> s_engine_logger{};
};
} // namespace weather
