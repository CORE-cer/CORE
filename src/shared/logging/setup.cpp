#include "setup.hpp"

#include <quill/core/Common.h>
#include <quill/core/LogLevel.h>
#include <quill/core/PatternFormatterOptions.h>
#include <quill/sinks/FileSink.h>
#include <quill/sinks/RotatingSink.h>
#include <quill/sinks/StreamSink.h>

#include <iostream>
#include <memory>
#include <utility>

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/RotatingFileSink.h"

namespace CORE::Internal::Logging {
quill::Logger* enable_logging_rotating() {
  std::cerr << "Logging to file: " << filename << std::endl;
  quill::Backend::start();

  // Frontend
  auto file_sink = quill::Frontend::create_or_get_sink<quill::RotatingFileSink>(

    filename,

    []()

    {
      quill::RotatingFileSinkConfig cfg;

      cfg.set_open_mode('w');

      cfg.set_filename_append_option(quill::FilenameAppendOption::StartDateTime);

      cfg.set_rotation_time_daily("18:30");

      cfg.set_rotation_max_file_size(2 << 24);

      return cfg;
    }(),

    quill::FileEventNotifier{});

  quill::Logger* logger = quill::Frontend::create_or_get_logger(
    "root",
    std::move(file_sink),

    quill::PatternFormatterOptions{"%(time) [%(thread_id)] %(short_source_location:<28) "

                                   "LOG_%(log_level:<9) %(logger:<12) %(message)",

                                   "%H:%M:%S.%Qns",
                                   quill::Timezone::GmtTime});

  // Change the LogLevel to print everything
  logger->set_log_level(quill::LogLevel::TraceL3);
  logger->init_backtrace(65536);

  return logger;
}

quill::Logger* enable_logging_stdout() {
  quill::Backend::start();

  // Frontend
  auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>(
    "sink_id_1");

  quill::Logger* logger = quill::Frontend::create_or_get_logger(
    "root",
    std::move(console_sink),

    quill::PatternFormatterOptions{"%(time) [%(thread_id)] %(short_source_location:<28) "

                                   "LOG_%(log_level:<9) %(logger:<12) %(message)",

                                   "%H:%M:%S.%Qns",
                                   quill::Timezone::GmtTime});

  logger->set_log_level(quill::LogLevel::Debug);
  logger->init_backtrace(65536);

  return logger;
}

quill::Logger* enable_logging_stdout_critical() {
  quill::Backend::start();

  // Frontend
  auto console_sink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>(
    "sink_id_1");

  quill::Logger* logger = quill::Frontend::create_or_get_logger(
    "root",
    std::move(console_sink),

    quill::PatternFormatterOptions{"%(time) [%(thread_id)] %(short_source_location:<28) "

                                   "LOG_%(log_level:<9) %(logger:<12) %(message)",

                                   "%H:%M:%S.%Qns",
                                   quill::Timezone::GmtTime});

  // Change the LogLevel to print everything
  logger->set_log_level(quill::LogLevel::TraceL3);
  logger->init_backtrace(65536);

  return logger;
}

}  // namespace CORE::Internal::Logging
