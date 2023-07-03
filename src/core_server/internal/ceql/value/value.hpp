#pragma once

#include <functional>
#include <memory>
#include <string>

#include "visitors/value_visitor.hpp"

namespace CORE {
namespace Internal {
namespace CEQL {
struct Value {
  /// All concrete classes of Value should have a constructor with no
  /// parameters, that way it can be serialized by cereal.

  virtual ~Value() noexcept {}

  /// noexcept is not added because some concrete classes utilize unique_ptr
  /// dereference and may eventually have exceptions.
  virtual std::string to_string() const = 0;
  virtual bool equals(Value* val) const noexcept = 0;
  virtual std::unique_ptr<Value> clone() const noexcept = 0;
  virtual void accept_visitor(ValueVisitor&) = 0;
};
}  // namespace CEQL
}  // namespace Internal
}  // namespace CORE
