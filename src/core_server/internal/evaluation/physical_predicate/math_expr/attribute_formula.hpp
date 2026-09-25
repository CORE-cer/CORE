#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"

namespace CORE::Internal::CEA {

// How reading an attribute is described to a FormulaBuilder, for every pairing of
//   GlobalType: the type the predicate works with (what the expression yields), and
//   LocalType:  the type the event actually stores at that position.
//
// This is the single place where those pairings are decided, shared by the two
// kinds of attribute node: Attribute<Global, Local> (strongly typed: position and
// stored type are fixed when the query is compiled) and NonStronglyTypedAttribute
// (weakly typed: they are looked up in the catalog, see that class).
//
//   Global  Local   formula
//   int64   int64   the attribute as an integer
//   double  double  the attribute as a double (NaN is modeled by the builder)
//   double  int64   the integer converted to double the way C++ does (rounding
//                   above 2^53, see FormulaBuilder::int_attribute_as_double)
//   other combinations: nullopt, so the atom stays opaque. Among them:
//     int64 <- double   C++ truncates the double, which the solver does not model
//     bool, strings, dates: not modeled
template <typename GlobalType, typename LocalType>
std::optional<FormulaBuilder::Handle>
attribute_formula(FormulaBuilder& builder,
                  Types::UniqueEventTypeId event_type,
                  size_t pos) {
  if constexpr (std::is_same_v<LocalType, int64_t>) {
    if constexpr (std::is_same_v<GlobalType, int64_t>) {
      return builder.int_attribute(event_type, pos);
    } else if constexpr (std::is_same_v<GlobalType, double>) {
      return builder.int_attribute_as_double(event_type, pos);
    } else {
      return std::nullopt;
    }
  } else if constexpr (std::is_same_v<GlobalType, double>
                       && std::is_same_v<LocalType, double>) {
    return builder.double_attribute(event_type, pos);
  } else {
    return std::nullopt;
  }
}

}  // namespace CORE::Internal::CEA
