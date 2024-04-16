#pragma once

#include <exception>
#include <string>

namespace CORE {

class StreamNotFoundException : public std::exception {
 public:
  StreamNotFoundException(std::string error_message) { m_message = error_message; }

  virtual const char* what() const noexcept override { return m_message.c_str(); }

 private:
  std::string m_message;  // Mensaje de la excepción
};
}  // namespace CORE
