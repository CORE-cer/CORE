#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <string>

#include "visitors/predicate_visitor.hpp"

namespace CORE::Internal::CEQL {

struct Predicate {
 public:
  // Max uint64_t corresponds to unset
  uint64_t physical_predicate_id = std::numeric_limits<uint64_t>::max();
  virtual ~Predicate() = default;
  virtual std::unique_ptr<Predicate> clone() const = 0;
  virtual std::unique_ptr<Predicate> negate() const = 0;
  virtual bool is_constant() const = 0;
  virtual bool equals(Predicate* other) const = 0;
  virtual std::string to_string() const = 0;
  virtual void accept_visitor(PredicateVisitor&) = 0;
};
}  // namespace CORE::Internal::CEQL
