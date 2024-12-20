#pragma once

#include <exception>
#include <string>

namespace CORE {

class WarningException : public std::exception {
 public:
  WarningException() = default;

  WarningException(std::string error_message) { m_message = error_message; }

  template <class Archive>
  void serialize(Archive& archive) {
    archive(m_message);
  }

  virtual const char* what() const noexcept override { return m_message.c_str(); }

 private:
  std::string m_message;
};
}  // namespace CORE
