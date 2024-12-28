#pragma once

#include <cstddef>
#include <exception>
#include <string>

namespace CORE {

class ParsingSyntaxException : public std::exception {
 public:
  ParsingSyntaxException() = default;

  ParsingSyntaxException(std::string error_message,
                         std::string offendingSymbol,
                         size_t line) {
    m_message = error_message + offendingSymbol + "\nLine: " + std::to_string(line);
  }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(m_message);
  }

  virtual const char* what() const noexcept override { return m_message.c_str(); }

 private:
  std::string m_message;
};
}  // namespace CORE
