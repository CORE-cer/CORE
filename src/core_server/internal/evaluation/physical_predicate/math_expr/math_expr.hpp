#pragma once

#include <memory>
#include <string>

#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class MathExpr {
 public:
  virtual std::unique_ptr<MathExpr<Type>> clone() const = 0;
  virtual ~MathExpr() = default;
  virtual Type eval(Types::EventWrapper&) = 0;
  virtual std::string to_string() const = 0;
};
}  // namespace CORE::Internal::CEA
