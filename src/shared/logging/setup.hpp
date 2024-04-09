#pragma once

#define QUILL_ROOT_LOGGER_ONLY
static char const* filename = "logfile.log";

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define LOG_L3_BACKTRACE(...)   \
  do {                          \
    LOG_BACKTRACE(__VA_ARGS__); \
    LOG_TRACE_L3(__VA_ARGS__);        \
  } while (0)
#else
#error "C++11 or later is required for variadic macro support."
#endif

namespace CORE::Internal::Logging {
void enable_logging_rotating();

void enable_logging_stdout();

void enable_logging_stdout_critical();
}  // namespace CORE::Internal::Logging
