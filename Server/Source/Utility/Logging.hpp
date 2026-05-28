#pragma once

#ifdef _DEBUG

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger
{
  static void Init()
  {
    spdlog::init_thread_pool(8192, 1);

    auto Logger = spdlog::basic_logger_mt<spdlog::async_factory>("global", "logs.txt");
    spdlog::set_default_logger(Logger);
    spdlog::set_level(spdlog::level::info);
  }

  template <typename... TArgs> static void Info(fmt::format_string<TArgs...> Format, TArgs &&...Args)
  {
    spdlog::info(Format, std::forward<TArgs>(Args)...);
  }

  template <typename... TArgs> static void Error(fmt::format_string<TArgs...> Format, TArgs &&...Args)
  {
    spdlog::error(Format, std::forward<TArgs>(Args)...);
  }

  template <typename... TArgs> static void Debug(fmt::format_string<TArgs...> Format, TArgs &&...Args)
  {
    spdlog::debug(Format, std::forward<TArgs>(Args)...);
  }
};

#define LOG_INFO(...) Logger::Info(__VA_ARGS__)
#define LOG_ERROR(...) Logger::Error(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::Debug(__VA_ARGS__)

#endif
