#pragma once

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

class Logger {
 public:
  static void Init() {
    auto console_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>("stdout");
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "file", "Logs.txt", true);

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

    auto logger = std::make_shared<spdlog::logger>(
        "MultiSink", sinks.begin(), sinks.end(), spdlog::thread_pool(),
        spdlog::async_overflow_policy::block);

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info);
  }

  template <typename... Args>
  static void Info(fmt::format_string<Args...> format, Args &&...args) {
    spdlog::info(format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void Error(fmt::format_string<Args...> format, Args &&...args) {
    spdlog::error(format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void Debug(fmt::format_string<Args...> format, Args &&...args) {
    spdlog::debug(format, std::forward<Args>(args)...);
  }
};

#define LOG_INFO(...)          \
  do {                         \
    Logger::Info(__VA_ARGS__); \
  } while (0)

#define LOG_ERROR(...)          \
  do {                          \
    Logger::Error(__VA_ARGS__); \
  } while (0)

#ifdef _DEBUG

#define LOG_DEBUG(...)          \
  do {                          \
    Logger::Debug(__VA_ARGS__); \
  } while (0)

#else  // not _DEBUG

// #define LOG_INFO(...) ((void)0)
// #define LOG_ERROR(...) ((void)0)
#define LOG_DEBUG(...) ((void)0)

#endif  // _DEBUG
