#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"

namespace CORE::Internal::CEA {

class PhysicalPredicate;

// What a physical predicate needs in order to describe itself as a logical
// formula, without knowing which solver (if any) will use the description.
//
// The minterm-tree optimization (src/core_server/internal/optimizations/) turns
// predicates into Z3 formulas to ask "can these two conditions both hold?". Each
// predicate class knows best what it means, so each has a `translate` method
// (see PhysicalPredicate) that builds its formula by calling this interface. The
// only implementation today lives in the optimization and uses Z3; because the
// predicates only see this abstract class, the engine itself does not depend on
// Z3 and builds without it.
//
// FORMULAS ARE HANDLES. A builder hands out opaque `Handle` values and knows
// what they stand for (an expression in its own representation). A predicate
// only passes handles back to the builder, never looks inside them, and a handle
// is only meaningful to the builder that produced it.
//
// SEMANTICS THE BUILDER MUST HONOUR (the predicates rely on them):
//   - values are int64_t or IEEE double, and comparisons follow C++: any
//     comparison with a NaN is false except `!=`;
//   - a builder may be less precise than reality but never stricter: it must not
//     claim a relation between two conditions that native evaluation can
//     contradict (see PhysicalPredicateZ3Translator for how Z3 is kept honest).
class FormulaBuilder {
 public:
  using Handle = std::size_t;

  FormulaBuilder() = default;
  FormulaBuilder(const FormulaBuilder&) = delete;
  FormulaBuilder& operator=(const FormulaBuilder&) = delete;
  virtual ~FormulaBuilder() = default;

  // ---- Numeric operands ----------------------------------------------------

  // The value of attribute `pos` of events of `event_type`. Atoms that read the
  // same attribute must get the same operand, so the solver can relate them.
  virtual Handle int_attribute(Types::UniqueEventTypeId event_type, std::size_t pos) = 0;
  virtual Handle
  double_attribute(Types::UniqueEventTypeId event_type, std::size_t pos) = 0;

  // An int64_t attribute as the double C++ converts it to (rounding above 2^53).
  virtual Handle
  int_attribute_as_double(Types::UniqueEventTypeId event_type, std::size_t pos) = 0;

  virtual Handle int_literal(int64_t value) = 0;

  // nullopt if `value` is not finite: NaN and infinity cannot be modeled, so the
  // predicate must then be left untranslated.
  virtual std::optional<Handle> double_literal(double value) = 0;

  // int64_t arithmetic (assumed not to overflow). Only linear uses are
  // translated by the predicates.
  virtual Handle add(Handle left, Handle right) = 0;
  virtual Handle sub(Handle left, Handle right) = 0;
  virtual Handle mul(Handle left, Handle right) = 0;

  // ---- Boolean formulas ----------------------------------------------------

  // `left <comparison> right` with C++ semantics for the operands' types.
  virtual Handle compare(ComparisonType comparison, Handle left, Handle right) = 0;

  virtual Handle constant_true() = 0;
  virtual Handle constant_false() = 0;
  virtual Handle conjunction(Handle left, Handle right) = 0;
  virtual Handle disjunction(Handle left, Handle right) = 0;
  virtual Handle negation(Handle operand) = 0;

  // A leaf predicate (atom) evaluated for events of `event_type`. The builder
  // asks the atom to translate itself (PhysicalPredicate::translate_atom) and, if
  // it cannot or fails, treats the atom as unrelated to everything else. The
  // answer is remembered per (atom, event type), so asking again for the same
  // atom returns the same formula.
  virtual Handle
  atom(const PhysicalPredicate& atom, Types::UniqueEventTypeId event_type) = 0;
};

}  // namespace CORE::Internal::CEA
