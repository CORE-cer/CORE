#include "setup.hpp"

#include <quill/Config.h>
#include <quill/LogLevel.h>
#include <quill/Quill.h>
#include <quill/handlers/RotatingFileHandler.h>

#include <memory>

namespace CORE::Internal::Logging {
void enable_logging_rotating() {
  // std::shared_ptr<quill::Handler> handler = quill::stdout_handler(); /** for stdout **/
  // std::shared_ptr<quill::Handler> handler = quill::file_handler("quickstart.log", "w");  /** for writing to file **/
  std::shared_ptr<quill::Handler> handler = quill::rotating_file_handler(filename, []() {
    quill::RotatingFileHandlerConfig cfg;
    cfg.set_rotation_time_daily("18:30");
    cfg.set_rotation_max_file_size(1'000'000'000);
    cfg.set_max_backup_files(3);
    return cfg;
  }());
  handler->set_pattern(
    "%(ascii_time) [%(thread)] %(fileline:<28) LOG_%(level_name) %(message)");

  // set configuration
  quill::Config cfg;
  cfg.default_handlers.push_back(handler);

  // Apply configuration and start the backend worker thread
  quill::configure(cfg);
  quill::start();

  auto logger = quill::get_root_logger();
  logger->set_log_level(quill::LogLevel::TraceL3);
  int size_backtrace = 1000;
  logger->init_backtrace(size_backtrace, quill::LogLevel::Critical);
}

void enable_logging_stdout() {
  if (quill::_g_root_logger != nullptr) {
    return;
  }
  std::shared_ptr<quill::Handler> handler = quill::stdout_handler();
  // std::shared_ptr<quill::Handler> handler = quill::file_handler("quickstart.log", "w");  /** for writing to file **/
  handler->set_pattern(
    "%(ascii_time) [%(thread)] %(fileline:<28) LOG_%(level_name) %(message)");

  // set configuration
  quill::Config cfg;
  cfg.default_handlers.push_back(handler);

  // Apply configuration and start the backend worker thread
  quill::configure(cfg);
  quill::start();

  auto logger = quill::get_root_logger();
  logger->set_log_level(quill::LogLevel::Debug);
  int size_backtrace = 10;
  logger->init_backtrace(size_backtrace, quill::LogLevel::Critical);
}

void enable_logging_stdout_critical() {
  if (quill::_g_root_logger != nullptr) {
    return;
  }
  std::shared_ptr<quill::Handler> handler = quill::stdout_handler();
  // std::shared_ptr<quill::Handler> handler = quill::file_handler("quickstart.log", "w");  /** for writing to file **/
  handler->set_pattern(
    "%(ascii_time) [%(thread)] %(fileline:<28) LOG_%(level_name) %(message)");

  // set configuration
  quill::Config cfg;
  cfg.default_handlers.push_back(handler);

  // Apply configuration and start the backend worker thread
  quill::configure(cfg);
  quill::start();

  auto logger = quill::get_root_logger();
  logger->set_log_level(quill::LogLevel::Critical);
  int size_backtrace = 10;
  logger->init_backtrace(size_backtrace, quill::LogLevel::Critical);
}

}  // namespace CORE::Internal::Logging
