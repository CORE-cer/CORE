#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <type_traits>

#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// Can an atom be evaluated on an event even when the baseline would have
// skipped it?
//
// WHY THIS MATTERS. The baseline evaluates lazily: And stops at its first false
// child and Or at its first true child, so `Integer2 != 0 AND Integer1 /
// Integer2 > 1` never divides by zero. The minterm tree evaluates the atoms on
// one root-to-leaf path instead, which can include the division for an event
// whose guard fails. For pure comparisons that is harmless (evaluating them
// only costs time), but some atoms can fail when evaluated:
//   - integer `/` and `%` trap on a zero divisor (SIGFPE) and on INT64_MIN / -1;
//   - `%` on doubles, and any arithmetic on non-numeric types, throws.
// A predicate containing such an atom is therefore not put in a tree: it is
// evaluated directly, exactly like the baseline, keeping the short-circuit.
//
// WHAT IS SAFE. Anything without a division or modulo, `/` on doubles (IEEE:
// gives inf or NaN, never traps), and integer `/` or `%` by a literal other
// than 0 and -1. A divisor that is an attribute or an expression is assumed
// unsafe, since its value is not known. The set of math nodes is closed
// (Addition, Subtraction, Multiplication, Division, Modulo; Literal, Attribute
// and NonStronglyTypedAttribute are leaves that cannot fail), so a node type
// that is added later must be handled in math_expr_is_speculation_safe.
namespace Detail {

// An integer divisor that can be neither 0 nor -1 (INT64_MIN / -1 traps too).
inline bool is_safe_integer_divisor(const CEA::MathExpr<int64_t>& divisor) {
  if (const auto* literal = dynamic_cast<const CEA::Literal<int64_t>*>(&divisor)) {
    return literal->val != 0 && literal->val != -1;
  }
  return false;
}

template <typename T>
bool math_expr_is_speculation_safe(const CEA::MathExpr<T>& expr);

template <typename T>
bool both_speculation_safe(const CEA::MathExpr<T>& left, const CEA::MathExpr<T>& right) {
  return math_expr_is_speculation_safe<T>(left) && math_expr_is_speculation_safe<T>(right);
}

template <typename T>
bool math_expr_is_speculation_safe(const CEA::MathExpr<T>& expr) {
  if (const auto* add = dynamic_cast<const CEA::Addition<T>*>(&expr)) {
    // Arithmetic on non-numeric types throws when evaluated.
    if constexpr (!std::is_arithmetic_v<T>)
      return false;
    else
      return both_speculation_safe<T>(add->left_expr(), add->right_expr());
  }
  if (const auto* sub = dynamic_cast<const CEA::Subtraction<T>*>(&expr)) {
    if constexpr (!std::is_arithmetic_v<T>)
      return false;
    else
      return both_speculation_safe<T>(*sub->left, *sub->right);
  }
  if (const auto* mul = dynamic_cast<const CEA::Multiplication<T>*>(&expr)) {
    if constexpr (!std::is_arithmetic_v<T>)
      return false;
    else
      return both_speculation_safe<T>(*mul->left, *mul->right);
  }
  if (const auto* div = dynamic_cast<const CEA::Division<T>*>(&expr)) {
    if constexpr (std::is_same_v<T, double>) {
      return both_speculation_safe<T>(*div->left, *div->right);  // IEEE, never traps
    } else if constexpr (std::is_same_v<T, int64_t>) {
      return is_safe_integer_divisor(*div->right)
             && both_speculation_safe<T>(*div->left, *div->right);
    } else {
      return false;  // bool division may divide by zero, others throw
    }
  }
  if (const auto* mod = dynamic_cast<const CEA::Modulo<T>*>(&expr)) {
    if constexpr (std::is_same_v<T, int64_t>) {
      return is_safe_integer_divisor(*mod->right)
             && both_speculation_safe<T>(*mod->left, *mod->right);
    } else {
      return false;  // Modulo<double> and other types throw when evaluated
    }
  }
  return true;  // Literal, Attribute, NonStronglyTypedAttribute
}

// The verdict for an atom holding math expressions of type T, or nullopt if the
// atom is not one (CompareMathExprs<*, T> or InRangePredicate<T>).
template <typename T>
std::optional<bool> math_atom_is_speculation_safe(const CEA::PhysicalPredicate* atom) {
  using C = CEA::ComparisonType;
  std::optional<bool> verdict;
  auto try_comparison = [&](auto comparison) {
    if (verdict.has_value()) return;
    constexpr C kComparison = decltype(comparison)::value;
    if (const auto* typed = dynamic_cast<const CEA::CompareMathExprs<kComparison, T>*>(
          atom)) {
      verdict = both_speculation_safe<T>(typed->left_expr(), typed->right_expr());
    }
  };
  try_comparison(std::integral_constant<C, C::EQUALS>{});
  try_comparison(std::integral_constant<C, C::GREATER>{});
  try_comparison(std::integral_constant<C, C::GREATER_EQUALS>{});
  try_comparison(std::integral_constant<C, C::LESS_EQUALS>{});
  try_comparison(std::integral_constant<C, C::LESS>{});
  try_comparison(std::integral_constant<C, C::NOT_EQUALS>{});
  if (verdict.has_value()) return verdict;

  if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, double>) {
    if (const auto* range = dynamic_cast<const CEA::InRangePredicate<T>*>(atom)) {
      return math_expr_is_speculation_safe<T>(range->value_expr())
             && math_expr_is_speculation_safe<T>(range->lower_bound_expr())
             && math_expr_is_speculation_safe<T>(range->upper_bound_expr());
    }
  }
  return std::nullopt;
}

}  // namespace Detail

// True if `atom` can be evaluated on any event of its admissible types without
// failing, whether or not the baseline would have evaluated it. Atoms that hold
// no math expression (comparisons with a constant, attribute vs attribute,
// regex) cannot fail.
inline bool speculation_safe(const CEA::PhysicalPredicate* atom) {
  if (auto verdict = Detail::math_atom_is_speculation_safe<int64_t>(atom))
    return *verdict;
  if (auto verdict = Detail::math_atom_is_speculation_safe<double>(atom)) return *verdict;
  if (auto verdict = Detail::math_atom_is_speculation_safe<std::string_view>(atom))
    return *verdict;
  if (auto verdict = Detail::math_atom_is_speculation_safe<bool>(atom)) return *verdict;
  return true;
}

}  // namespace CORE::Internal::Optimizations::MintermTree
