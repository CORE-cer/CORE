#pragma once

#include <functional>
#include <memory>
#include <string>

namespace CORETypes {
struct Value {
  /// All concrete classes of Value should have a constructor with no
  /// parameters, that way it can be serialized by cereal.

  virtual ~Value() noexcept {}

  /// noexcept is not added because some concrete classes utilize unique_ptr
  /// dereference and may eventually have exceptions.
  virtual std::string to_string() const = 0;

  virtual bool equals (Value* val) const noexcept = 0;

  virtual std::unique_ptr<Value> clone() const noexcept = 0;

  /**
   * IMPORTANT:
   * Note that all concrete classes of Value should include the function
   * serialize so that cereal works correctly. And, this polymorphic
   * relationship should be added to the cereal_serializer.hpp file if it
   * is not already.
   */
};
}  // namespace CORETypes
