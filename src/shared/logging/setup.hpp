#pragma once

#include <quill/Logger.h>
static char const* filename = "logs/logfile.log";

namespace CORE::Internal::Logging {
quill::Logger* enable_logging_rotating();

quill::Logger* enable_logging_stdout();

quill::Logger* enable_logging_stdout_critical();
}  // namespace CORE::Internal::Logging
