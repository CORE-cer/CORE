#pragma once

#include <exception>

namespace CORE {

class ParsingSyntaxException : public std::exception {
 public:
  ParsingSyntaxException(std::string error_message,
                         std::string offendingSymbol,
                         size_t line) {
    m_message = error_message + offendingSymbol + "\nLine: " + std::to_string(line);
  }

  virtual const char* what() const noexcept override { return m_message.c_str(); }

 private:
  std::string m_message;  // Mensaje de la excepción
};
}  // namespace CORE
