#pragma once

#define QUILL_ROOT_LOGGER_ONLY
#include <gmpxx.h>
#include <quill/Quill.h>
#include <quill/detail/LogMacros.h>


#include <exception>
#include <string>

namespace CORE {

class StreamNameAlreadyDeclaredException : public std::exception {
 public:
  StreamNameAlreadyDeclaredException(std::string error_message) {
    m_message = error_message;
    LOG_INFO("User made an invalid query. This message was sent to client: \n'{}'", error_message);
  }

  virtual const char* what() const noexcept override { return m_message.c_str(); }

 private:
  std::string m_message;
};
}  // namespace CORE
