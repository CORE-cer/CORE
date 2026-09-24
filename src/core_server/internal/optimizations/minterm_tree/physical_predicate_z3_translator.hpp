#pragma once

#include <z3++.h>

#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <system_error>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/literal.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/math_expr.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// Turns CORE's PhysicalPredicate trees into Z3 formulas so that the solver can
// answer questions like "can `x > 100` and `NOT x > 50` both hold?".
// It runs while a query is compiled and is never used per event.
//
// WHY A TRANSLATION IS NEEDED: a PhysicalPredicate is an opaque C++ object with
// an eval(event) method; Z3 cannot look inside it. The translator rebuilds the
// same condition as a formula over symbolic variables, one variable per
// (event type, attribute position), so atoms that read the same attribute share
// a variable and Z3 can relate them.
//
// THE SAFE SUBSET: Z3 computes with mathematical integers and reals, CORE with
// int64_t and IEEE doubles. The two only agree on some operations, and the
// solver must never believe something native evaluation would contradict, so
// only those are translated:
//   - comparisons (== != < <= > >=) between attributes and literals, int64_t or
//     double, and IN RANGE over them. NaN is modeled explicitly (see Term).
//   - int64_t + and - between attributes/literals, and * by a literal
//     (linear arithmetic; assumes no int64_t overflow).
//   - an int64_t attribute used where a double is expected (int vs double
//     comparisons, int attributes inside double expressions). C++ converts by
//     rounding, which is exact only for |value| <= 2^53, so beyond that range
//     the converted value is only bounded, not known (see int_attribute_as_double).
// Everything else becomes an OPAQUE atom: a fresh, unconstrained Z3 boolean.
// An opaque atom is still evaluated natively at runtime; it just cannot be
// related to other atoms. Opaque on purpose:
//   - int64_t / and %:      Z3's div/mod are Euclidean, C++ truncates toward
//                           zero (they differ for negative operands).
//   - any double arithmetic: IEEE rounding differs from exact real arithmetic
//                           (0.2 + 0.1 == 0.3 is true in Z3, false in C++).
//   - int64_t * int64_t:    nonlinear, Z3 may answer "unknown".
//   - strings, bool, dates, regex/LIKE: not modeled.
//   - weakly-typed attributes: resolved by NAME at runtime, so the attribute
//                           position (and thus the Z3 variable) differs per
//                           event type.
class PhysicalPredicateZ3Translator {
 public:
  explicit PhysicalPredicateZ3Translator(z3::context& ctx) : ctx_(ctx) {}

  // Translates one atom (a non-compound predicate) as seen by events of
  // `event_type`. Results are cached per (atom, event type).
  z3::expr translate_atom(const CEA::PhysicalPredicate* atom,
                          Types::UniqueEventTypeId event_type) {
    AtomKey key{atom, event_type};
    auto cached = atom_cache_.find(key);
    if (cached != atom_cache_.end()) return cached->second;

    std::optional<z3::expr> translated;
    try {
      translated = try_translate_atom(atom, event_type);
    } catch (const z3::exception&) {
      // Z3 rejected something (e.g. a malformed numeral): stay safe and opaque.
      translated = std::nullopt;
    }

    if (!translated.has_value()) opaque_atoms_.insert(atom);
    z3::expr result = translated.has_value() ? *translated : fresh_opaque_bool();
    atom_cache_.emplace(key, result);
    return result;
  }

  // Translates a whole (possibly compound) top-level predicate into one
  // formula, mirroring EXACTLY how it is evaluated at runtime:
  //   - `gated` says whether evaluation goes through PhysicalPredicate::
  //     operator(), which returns false for event types the node does not
  //     admit. The top-level predicate and the children of an Or are gated;
  //     the children of an And/Not are evaluated directly with eval() and are
  //     not. Getting this wrong would make the formula true for events the
  //     baseline evaluates to false.
  z3::expr translate_formula(const CEA::PhysicalPredicate* node,
                             Types::UniqueEventTypeId event_type,
                             bool gated = true) {
    if (gated && !node->admits_any_event_type
        && !node->admissible_event_types.contains(event_type)) {
      return ctx_.bool_val(false);
    }

    if (node->is_compound()) {
      if (const auto* not_node = dynamic_cast<const CEA::NotPredicate*>(node)) {
        return !translate_formula(not_node->get_children().front(), event_type, false);
      }
      if (const auto* and_node = dynamic_cast<const CEA::AndPredicate*>(node)) {
        return combine(and_node->get_children(), event_type, /*is_and=*/true, false);
      }
      if (const auto* or_node = dynamic_cast<const CEA::OrPredicate*>(node)) {
        return combine(or_node->get_children(), event_type, /*is_and=*/false, true);
      }
    }
    return translate_atom(node, event_type);
  }

  // How many distinct atoms could not be modeled for at least one event type
  // (for logging/diagnostics). An atom that is opaque for two event types still
  // counts once.
  size_t opaque_atom_count() const { return opaque_atoms_.size(); }

  // The Z3 variables that stand for an event's attribute values. They are
  // public so that tests can pin a concrete event's values onto a translated
  // formula and check it against native evaluation.

  // One Z3 variable per (event type, attribute position, sort), shared by all
  // atoms so that Z3 can see they talk about the same attribute.
  z3::expr
  attribute_symbol(Types::UniqueEventTypeId event_type, size_t pos, bool is_real) {
    AttributeKey key{event_type, pos, is_real};
    auto cached = attribute_symbol_cache_.find(key);
    if (cached != attribute_symbol_cache_.end()) return cached->second;

    std::string name = "attr_" + std::to_string(event_type) + "_" + std::to_string(pos)
                       + (is_real ? "_r" : "_i");
    z3::expr symbol = is_real ? ctx_.real_const(name.c_str())
                              : ctx_.int_const(name.c_str());
    attribute_symbol_cache_.emplace(key, symbol);
    return symbol;
  }

  // The "this double attribute is NaN" flag for an attribute.
  z3::expr nan_flag(Types::UniqueEventTypeId event_type, size_t pos) {
    AttributeKey key{event_type, pos, true};
    auto cached = nan_flag_cache_.find(key);
    if (cached != nan_flag_cache_.end()) return cached->second;

    std::string name = "nan_" + std::to_string(event_type) + "_" + std::to_string(pos);
    z3::expr flag = ctx_.bool_const(name.c_str());
    nan_flag_cache_.emplace(key, flag);
    return flag;
  }

 private:
  // A translated numeric operand. `may_be_nan` is only set for double
  // attributes: NaN makes every comparison false except !=, which plain real
  // arithmetic cannot express, so each double attribute gets a boolean
  // "is NaN" flag that comparisons take into account.
  struct Term {
    z3::expr value;
    std::optional<z3::expr> may_be_nan;
  };

  struct AtomKey {
    const CEA::PhysicalPredicate* atom;
    Types::UniqueEventTypeId event_type;

    bool operator==(const AtomKey& other) const {
      return atom == other.atom && event_type == other.event_type;
    }
  };

  struct AtomKeyHash {
    std::size_t operator()(const AtomKey& key) const {
      return std::hash<const CEA::PhysicalPredicate*>()(key.atom)
             ^ (std::hash<Types::UniqueEventTypeId>()(key.event_type) << 1);
    }
  };

  struct AttributeKey {
    Types::UniqueEventTypeId event_type;
    size_t pos;
    bool is_real;

    bool operator==(const AttributeKey& other) const {
      return event_type == other.event_type && pos == other.pos
             && is_real == other.is_real;
    }
  };

  struct AttributeKeyHash {
    std::size_t operator()(const AttributeKey& key) const {
      std::size_t h = std::hash<Types::UniqueEventTypeId>()(key.event_type);
      h ^= std::hash<size_t>()(key.pos) + 0x9e3779b9 + (h << 6) + (h >> 2);
      h ^= std::hash<bool>()(key.is_real) + 0x9e3779b9 + (h << 6) + (h >> 2);
      return h;
    }
  };

  // Every integer with |value| <= 2^53 is exactly representable as a double.
  static constexpr int64_t kMaxExactInt = int64_t{1} << 53;

  z3::context& ctx_;
  std::unordered_map<AtomKey, z3::expr, AtomKeyHash> atom_cache_;
  std::unordered_map<AttributeKey, z3::expr, AttributeKeyHash> attribute_symbol_cache_;
  std::unordered_map<AttributeKey, z3::expr, AttributeKeyHash> nan_flag_cache_;
  std::unordered_map<AttributeKey, z3::expr, AttributeKeyHash> conversion_symbol_cache_;
  uint64_t opaque_counter_ = 0;
  std::unordered_set<const CEA::PhysicalPredicate*> opaque_atoms_;

  // ---- Building blocks ----------------------------------------------------

  z3::expr combine(const std::vector<CEA::PhysicalPredicate*>& children,
                   Types::UniqueEventTypeId event_type,
                   bool is_and,
                   bool children_gated) {
    z3::expr acc = ctx_.bool_val(is_and);  // neutral element: true for And, false for Or
    bool first = true;
    for (CEA::PhysicalPredicate* child : children) {
      z3::expr child_expr = translate_formula(child, event_type, children_gated);
      if (first) {
        acc = child_expr;
        first = false;
      } else {
        acc = is_and ? (acc && child_expr) : (acc || child_expr);
      }
    }
    return acc;
  }

  // A brand-new boolean about which Z3 knows nothing (see "opaque" above).
  z3::expr fresh_opaque_bool() {
    std::string name = "opaque_" + std::to_string(opaque_counter_++);
    return ctx_.bool_const(name.c_str());
  }

  // An attribute as an operand. ValueType is the attribute's own C++ type.
  template <typename ValueType>
  Term attribute_term(Types::UniqueEventTypeId event_type, size_t pos) {
    if constexpr (std::is_same_v<ValueType, double>) {
      return Term{attribute_symbol(event_type, pos, true), nan_flag(event_type, pos)};
    } else {
      return Term{attribute_symbol(event_type, pos, false), std::nullopt};
    }
  }

  // An int64_t attribute as the double C++ would convert it to.
  //
  // C++ rounds the conversion to the nearest double, which is exact only while
  // |value| <= 2^53 (beyond it, neighbouring ints collapse: (double)(2^53 + 1)
  // == 2^53). Z3's to_real is always exact, so using it alone would let the
  // solver decide `Integer1 > Double1` differently from the real evaluation. So:
  //   |value| <= 2^53   the exact value, to_real(value);
  //   value  >  2^53    some real >= 2^53   (rounding is monotone and 2^53 is
  //                     itself a double, so the result cannot be smaller);
  //   value  < -2^53    some real <= -2^53  (likewise).
  // "Some real" is a free variable, shared by every atom reading the same
  // attribute since they all see the same rounded value. This only ever makes
  // the model less precise (more outcomes possible), never stricter, and it
  // costs nothing at runtime.
  Term int_attribute_as_double(Types::UniqueEventTypeId event_type, size_t pos) {
    z3::expr integer = attribute_symbol(event_type, pos, false);
    z3::expr max_exact = ctx_.int_val(kMaxExactInt);
    z3::expr min_exact = ctx_.int_val(-kMaxExactInt);
    z3::expr max_exact_real = z3::to_real(max_exact);
    z3::expr min_exact_real = z3::to_real(min_exact);

    z3::expr rounded = conversion_symbol(event_type, pos);
    z3::expr at_least_max = z3::ite(rounded >= max_exact_real, rounded, max_exact_real);
    z3::expr at_most_min = z3::ite(rounded <= min_exact_real, rounded, min_exact_real);
    return Term{z3::ite(integer > max_exact,
                        at_least_max,
                        z3::ite(integer < min_exact, at_most_min, z3::to_real(integer))),
                std::nullopt};
  }

  // The unconstrained "rounded value" of an int attribute beyond 2^53.
  z3::expr conversion_symbol(Types::UniqueEventTypeId event_type, size_t pos) {
    AttributeKey key{event_type, pos, true};
    auto cached = conversion_symbol_cache_.find(key);
    if (cached != conversion_symbol_cache_.end()) return cached->second;

    std::string name = "conv_" + std::to_string(event_type) + "_" + std::to_string(pos);
    z3::expr symbol = ctx_.real_const(name.c_str());
    conversion_symbol_cache_.emplace(key, symbol);
    return symbol;
  }

  // One operand of an attribute-vs-attribute comparison. An int64_t compared
  // with a double is converted to double first (as C++ does).
  template <typename Type, typename OtherType>
  Term comparison_operand(Types::UniqueEventTypeId event_type, size_t pos) {
    if constexpr (std::is_same_v<Type, int64_t> && std::is_same_v<OtherType, double>) {
      return int_attribute_as_double(event_type, pos);
    } else {
      return attribute_term<Type>(event_type, pos);
    }
  }

  std::optional<Term> literal_term(int64_t value) {
    return Term{ctx_.int_val(value), std::nullopt};
  }

  // Doubles are written as the shortest decimal that round-trips, in plain
  // (non-scientific) notation so Z3's parser accepts any magnitude. That keeps
  // the ordering between distinct double literals exact. NaN/infinity cannot be
  // a Z3 real, so such literals make the atom opaque.
  std::optional<Term> literal_term(double value) {
    if (!std::isfinite(value)) return std::nullopt;
    char buffer[512];
    auto [end, error] = std::to_chars(buffer,
                                      buffer + sizeof(buffer),
                                      value,
                                      std::chars_format::fixed);
    if (error != std::errc()) return std::nullopt;
    return Term{ctx_.real_val(std::string(buffer, end).c_str()), std::nullopt};
  }

  // Z3 needs both sides of a comparison to share a sort. Every int64_t -> double
  // conversion is made explicitly beforehand (int_attribute_as_double), so a mix
  // reaching this point would be an exact, and therefore unsound, conversion.
  // Refuse it: translate_atom turns the exception into an opaque atom.
  static std::pair<z3::expr, z3::expr> promote(z3::expr lhs, z3::expr rhs) {
    if (lhs.is_real() != rhs.is_real()) {
      throw z3::exception("int and real operands must be converted explicitly");
    }
    return {lhs, rhs};
  }

  // `lhs <Comp> rhs` with C++/IEEE semantics for NaN: any comparison involving
  // NaN is false, except != which is true.
  template <CEA::ComparisonType Comp>
  static z3::expr compare(const Term& lhs, const Term& rhs) {
    auto [left, right] = promote(lhs.value, rhs.value);

    z3::expr result = left != right;
    if constexpr (Comp == CEA::ComparisonType::EQUALS)
      result = left == right;
    else if constexpr (Comp == CEA::ComparisonType::GREATER)
      result = left > right;
    else if constexpr (Comp == CEA::ComparisonType::GREATER_EQUALS)
      result = left >= right;
    else if constexpr (Comp == CEA::ComparisonType::LESS_EQUALS)
      result = left <= right;
    else if constexpr (Comp == CEA::ComparisonType::LESS)
      result = left < right;

    for (const Term* term : {&lhs, &rhs}) {
      if (!term->may_be_nan.has_value()) continue;
      if constexpr (Comp == CEA::ComparisonType::NOT_EQUALS) {
        result = result || *term->may_be_nan;
      } else {
        result = result && !*term->may_be_nan;
      }
    }
    return result;
  }

  // ---- Math expressions ---------------------------------------------------

  // Translates a MathExpr tree, or returns nullopt if any part of it is outside
  // the safe subset (which makes the whole containing atom opaque).
  template <typename ValueType>
  std::optional<Term> translate_math_expr(const CEA::MathExpr<ValueType>& node,
                                          Types::UniqueEventTypeId event_type) {
    if (const auto* literal = dynamic_cast<const CEA::Literal<ValueType>*>(&node)) {
      return literal_term(static_cast<ValueType>(literal->val));
    }

    // An int64_t attribute; in a double expression it is converted the way C++
    // converts it (see int_attribute_as_double).
    if (const auto* attr = dynamic_cast<const CEA::Attribute<ValueType, int64_t>*>(&node)) {
      if constexpr (std::is_same_v<ValueType, double>) {
        return int_attribute_as_double(event_type, attr->pos);
      } else {
        return Term{attribute_symbol(event_type, attr->pos, false), std::nullopt};
      }
    }

    if constexpr (std::is_same_v<ValueType, double>) {
      if (const auto* attr = dynamic_cast<const CEA::Attribute<double, double>*>(&node)) {
        return attribute_term<double>(event_type, attr->pos);
      }
      // Any arithmetic over doubles is deliberately not translated (IEEE
      // rounding differs from exact real arithmetic).
      return std::nullopt;
    } else {
      if (const auto* add = dynamic_cast<const CEA::Addition<int64_t>*>(&node)) {
        return combine_terms(add->left_expr(), add->right_expr(), event_type, '+');
      }
      if (const auto* sub = dynamic_cast<const CEA::Subtraction<int64_t>*>(&node)) {
        return combine_terms(*sub->left, *sub->right, event_type, '-');
      }
      if (const auto* mul = dynamic_cast<const CEA::Multiplication<int64_t>*>(&node)) {
        // Only multiplication by a literal is linear; attribute * attribute is
        // nonlinear and Z3 may not be able to decide it.
        bool has_literal_side = dynamic_cast<const CEA::Literal<int64_t>*>(mul->left.get())
                                || dynamic_cast<const CEA::Literal<int64_t>*>(
                                  mul->right.get());
        if (!has_literal_side) return std::nullopt;
        return combine_terms(*mul->left, *mul->right, event_type, '*');
      }
      // Division and modulo (and anything else, e.g. weakly-typed attributes)
      // are not translated.
      return std::nullopt;
    }
  }

  // Translates both operands of an int64_t operator and combines them.
  std::optional<Term> combine_terms(const CEA::MathExpr<int64_t>& left,
                                    const CEA::MathExpr<int64_t>& right,
                                    Types::UniqueEventTypeId event_type,
                                    char op) {
    auto l = translate_math_expr<int64_t>(left, event_type);
    auto r = translate_math_expr<int64_t>(right, event_type);
    if (!l || !r) return std::nullopt;
    if (op == '+') return Term{l->value + r->value, std::nullopt};
    if (op == '-') return Term{l->value - r->value, std::nullopt};
    return Term{l->value * r->value, std::nullopt};
  }

  // ---- Atom families ------------------------------------------------------
  // PhysicalPredicate leaves are templates over the comparison and the value
  // type, so the concrete type is recovered by trying each combination with
  // dynamic_cast (fine here: this only runs at query compile time).

  // Calls `try_one` once per comparison type (as a compile-time constant) until
  // one of them recognizes the atom.
  template <typename TryOne>
  static std::optional<z3::expr> try_each_comparison(TryOne try_one) {
    using C = CEA::ComparisonType;
    if (auto r = try_one(std::integral_constant<C, C::EQUALS>{})) return r;
    if (auto r = try_one(std::integral_constant<C, C::GREATER>{})) return r;
    if (auto r = try_one(std::integral_constant<C, C::GREATER_EQUALS>{})) return r;
    if (auto r = try_one(std::integral_constant<C, C::LESS_EQUALS>{})) return r;
    if (auto r = try_one(std::integral_constant<C, C::LESS>{})) return r;
    if (auto r = try_one(std::integral_constant<C, C::NOT_EQUALS>{})) return r;
    return std::nullopt;
  }

  // `attribute <Comp> constant`
  template <CEA::ComparisonType Comp, typename ValueType>
  std::optional<z3::expr> try_compare_with_constant(const CEA::PhysicalPredicate* atom,
                                                    Types::UniqueEventTypeId event_type) {
    const auto* typed = dynamic_cast<const CEA::CompareWithConstant<Comp, ValueType>*>(
      atom);
    if (!typed) return std::nullopt;
    std::optional<Term> constant = literal_term(typed->constant());
    if (!constant) return std::nullopt;
    return compare<Comp>(attribute_term<ValueType>(event_type, typed->position()),
                         *constant);
  }

  // `attribute <Comp> attribute`
  template <CEA::ComparisonType Comp, typename LeftType, typename RightType>
  std::optional<z3::expr> try_compare_with_attribute(const CEA::PhysicalPredicate* atom,
                                                     Types::UniqueEventTypeId event_type) {
    const auto*
      typed = dynamic_cast<const CEA::CompareWithAttribute<Comp, LeftType, RightType>*>(
        atom);
    if (!typed) return std::nullopt;
    return compare<Comp>(comparison_operand<LeftType, RightType>(event_type,
                                                                 typed->left_position()),
                         comparison_operand<RightType, LeftType>(event_type,
                                                                 typed->right_position()));
  }

  // `math_expr <Comp> math_expr`
  template <CEA::ComparisonType Comp, typename ValueType>
  std::optional<z3::expr> try_compare_math_exprs(const CEA::PhysicalPredicate* atom,
                                                 Types::UniqueEventTypeId event_type) {
    const auto* typed = dynamic_cast<const CEA::CompareMathExprs<Comp, ValueType>*>(atom);
    if (!typed) return std::nullopt;
    auto lhs = translate_math_expr<ValueType>(typed->left_expr(), event_type);
    auto rhs = translate_math_expr<ValueType>(typed->right_expr(), event_type);
    if (!lhs || !rhs) return std::nullopt;
    return compare<Comp>(*lhs, *rhs);
  }

  // `math_expr IN RANGE (lower, upper)`, both bounds inclusive.
  template <typename ValueType>
  std::optional<z3::expr>
  try_in_range(const CEA::PhysicalPredicate* atom, Types::UniqueEventTypeId event_type) {
    const auto* typed = dynamic_cast<const CEA::InRangePredicate<ValueType>*>(atom);
    if (!typed) return std::nullopt;
    auto value = translate_math_expr<ValueType>(typed->value_expr(), event_type);
    auto lower = translate_math_expr<ValueType>(typed->lower_bound_expr(), event_type);
    auto upper = translate_math_expr<ValueType>(typed->upper_bound_expr(), event_type);
    if (!value || !lower || !upper) return std::nullopt;
    return compare<CEA::ComparisonType::GREATER_EQUALS>(*value, *lower)
           && compare<CEA::ComparisonType::LESS_EQUALS>(*value, *upper);
  }

  std::optional<z3::expr> try_translate_atom(const CEA::PhysicalPredicate* atom,
                                             Types::UniqueEventTypeId event_type) {
    // Value types other than int64_t/double (string, bool, date) are never
    // tried, so those atoms, and regex/LIKE, fall through to opaque.
    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_with_constant<decltype(comp)::value, int64_t>(atom,
                                                                           event_type);
        }))
      return r;
    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_with_constant<decltype(comp)::value, double>(atom,
                                                                          event_type);
        }))
      return r;

    // Attribute vs attribute: every int64_t/double pairing.
    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_with_attribute<decltype(comp)::value, int64_t, int64_t>(
            atom, event_type);
        }))
      return r;
    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_with_attribute<decltype(comp)::value, int64_t, double>(
            atom, event_type);
        }))
      return r;
    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_with_attribute<decltype(comp)::value, double, int64_t>(
            atom, event_type);
        }))
      return r;
    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_with_attribute<decltype(comp)::value, double, double>(
            atom, event_type);
        }))
      return r;

    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_math_exprs<decltype(comp)::value, int64_t>(atom, event_type);
        }))
      return r;
    if (auto r = try_each_comparison([&](auto comp) {
          return try_compare_math_exprs<decltype(comp)::value, double>(atom, event_type);
        }))
      return r;

    if (auto r = try_in_range<int64_t>(atom, event_type)) return r;
    if (auto r = try_in_range<double>(atom, event_type)) return r;

    return std::nullopt;
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree
