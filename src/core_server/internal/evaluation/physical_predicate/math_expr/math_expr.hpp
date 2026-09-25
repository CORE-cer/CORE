#pragma once

#include <memory>
#include <optional>
#include <string>

#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename Type>
class MathExpr {
 public:
  virtual std::unique_ptr<MathExpr<Type>> clone() const = 0;
  virtual ~MathExpr() = default;
  virtual Type eval(Types::EventWrapper&) = 0;
  virtual std::string to_string() const = 0;

  // This expression as a formula (see FormulaBuilder), or nullopt if it cannot
  // be described exactly, which keeps the atom that contains it opaque. The
  // default is "cannot": only the node types whose C++ semantics match the
  // solver's override it (see FormulaBuilder for the rules).
  virtual std::optional<FormulaBuilder::Handle>
  translate(FormulaBuilder& /*builder*/, Types::UniqueEventTypeId /*event_type*/) const {
    return std::nullopt;
  }
};
}  // namespace CORE::Internal::CEA
