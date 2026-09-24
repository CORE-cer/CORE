#pragma once

// Builders for hand-made PhysicalPredicates and math expressions, shared by the
// minterm-tree tests that do not go through CEQL. Every predicate is built for
// the fixture's event types (see test_support.hpp):
//   event type 0: String, Integer1, Integer2, Double1, Double2  (positions 0..4)
//   event type 1: Integer1, Integer2                            (positions 0..1)

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <set>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

using Comparison = CEA::ComparisonType;
using Atom = std::unique_ptr<CEA::PhysicalPredicate>;
template <typename T>
using Expr = std::unique_ptr<CEA::MathExpr<T>>;

// Attribute positions of the fixture's event type 0 (see test_support.hpp):
// String, Integer1, Integer2, Double1, Double2.
inline constexpr size_t kString = 0;
inline constexpr size_t kInteger1 = 1;
inline constexpr size_t kInteger2 = 2;
inline constexpr size_t kDouble1 = 3;
inline constexpr size_t kDouble2 = 4;

inline constexpr int64_t k2p53 = int64_t{1}
                                 << 53;  // largest range where int -> double is exact
inline constexpr double kInf = std::numeric_limits<double>::infinity();
inline constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();
inline constexpr double kDoubleMax = std::numeric_limits<double>::max();
inline constexpr int64_t kInt64Min = std::numeric_limits<int64_t>::min();
inline constexpr int64_t kInt64Max = std::numeric_limits<int64_t>::max();

// Calls `visit` once per comparison, with the comparison as a compile-time
// constant so it can be used as a template argument.
template <typename Visit>
inline void for_each_comparison(Visit&& visit) {
  visit(std::integral_constant<Comparison, Comparison::EQUALS>{});
  visit(std::integral_constant<Comparison, Comparison::GREATER>{});
  visit(std::integral_constant<Comparison, Comparison::GREATER_EQUALS>{});
  visit(std::integral_constant<Comparison, Comparison::LESS_EQUALS>{});
  visit(std::integral_constant<Comparison, Comparison::LESS>{});
  visit(std::integral_constant<Comparison, Comparison::NOT_EQUALS>{});
}

// ---- Small builders for math expressions -----------------------------------

template <typename T>
Expr<T> literal(T value) {
  return std::make_unique<CEA::Literal<T>>(value);
}

// An attribute read: `Stored` is the type the event really holds at `pos`, `T`
// the type the expression yields (they differ for an int used in a double
// expression).
template <typename T, typename Stored = T>
Expr<T> attribute(size_t pos) {
  return std::make_unique<CEA::Attribute<T, Stored>>(pos);
}

template <template <typename> class Node, typename T>
Expr<T> node(Expr<T> left, Expr<T> right) {
  return std::make_unique<Node<T>>(std::move(left), std::move(right));
}

// ---- Small builders for atoms ----------------------------------------------

// std::vector<Atom> from a list (unique_ptrs cannot come out of a braced list).
template <typename... Children>
std::vector<Atom> children_of(Children&&... children) {
  std::vector<Atom> out;
  (out.push_back(std::forward<Children>(children)), ...);
  return out;
}

// `attribute[position] > threshold` (or <), admitting the given event types.
inline Atom greater_at(std::set<uint64_t> types, size_t position, int64_t threshold) {
  return std::make_unique<CEA::CompareWithConstant<Comparison::GREATER, int64_t>>(
    std::move(types), position, threshold);
}

inline Atom less_at(std::set<uint64_t> types, size_t position, int64_t threshold) {
  return std::make_unique<CEA::CompareWithConstant<Comparison::LESS, int64_t>>(std::move(
                                                                                 types),
                                                                               position,
                                                                               threshold);
}

// `attribute[position] > threshold` on a double attribute.
inline Atom
double_greater_at(std::set<uint64_t> types, size_t position, double threshold) {
  return std::make_unique<CEA::CompareWithConstant<Comparison::GREATER, double>>(
    std::move(types), position, threshold);
}

// A string equality: never modeled, so it is opaque, and (unlike the regex
// predicate) it can admit several event types.
inline Atom string_equals(std::set<uint64_t> types, const char* text) {
  return std::make_unique<CEA::CompareWithConstant<Comparison::EQUALS, std::string_view>>(
    std::move(types), kString, std::string_view(text));
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
