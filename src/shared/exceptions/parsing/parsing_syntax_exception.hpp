#pragma once

#define QUILL_ROOT_LOGGER_ONLY
#include <gmpxx.h>
#include <quill/Quill.h>
#include <quill/detail/LogMacros.h>


#include <exception>

namespace CORE {

class ParsingSyntaxException : public std::exception {
 public:
  ParsingSyntaxException(std::string error_message,
                         std::string offendingSymbol,
                         size_t line) {
    m_message = error_message + offendingSymbol + "\nLine: " + std::to_string(line);
    LOG_INFO("User made an invalid query. This message was sent to client: \n'{}'", error_message);
  }

  virtual const char* what() const noexcept override { return m_message.c_str(); }

 private:
  std::string m_message;
};
}  // namespace CORE
