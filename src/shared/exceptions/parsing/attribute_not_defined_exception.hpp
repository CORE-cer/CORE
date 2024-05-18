#pragma once

#include <exception>
#include <string>

namespace CORE {

class AttributeNotDefinedException : public std::exception {
 public:
  AttributeNotDefinedException(std::string error_message) { m_message = error_message; }

  virtual const char* what() const noexcept override { return m_message.c_str(); }

 private:
  std::string m_message;
};
}  // namespace CORE