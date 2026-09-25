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
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"

namespace CORE::Internal::Optimizations::MintermTree {

// The Z3 side of the translation of CORE's PhysicalPredicates into logical
// formulas, so that the solver can answer questions like "can `x > 100` and
// `NOT x > 50` both hold?". It runs while a query is compiled and is never used
// per event.
//
// WHO DOES WHAT. Each predicate class describes ITSELF: PhysicalPredicate::
// translate (and translate_atom / MathExpr::translate) walks the predicate and
// calls the abstract CEA::FormulaBuilder to say "this is attribute 3 of event
// type 0 compared with 100". This class is the FormulaBuilder that turns those
// calls into Z3 expressions. Predicates never see Z3, so the engine builds
// without it; everything Z3-specific is here.
//
// A PhysicalPredicate is an opaque C++ object with an eval(event) method; Z3
// cannot look inside it. The translation rebuilds the same condition as a
// formula over symbolic variables, one variable per (event type, attribute
// position), so atoms that read the same attribute share a variable and Z3 can
// relate them.
//
// THE SAFE SUBSET: Z3 computes with mathematical integers and reals, CORE with
// int64_t and IEEE doubles. The two only agree on some operations, and the
// solver must never believe something native evaluation would contradict, so
// only those are translated (each predicate class decides for itself, see its
// translate method):
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
class PhysicalPredicateZ3Translator : public CEA::FormulaBuilder {
 public:
  explicit PhysicalPredicateZ3Translator(z3::context& ctx) : ctx_(ctx) {}

  // Translates one atom (a non-compound predicate) as seen by events of
  // `event_type`. Results are cached per (atom, event type).
  z3::expr translate_atom(const CEA::PhysicalPredicate* atom,
                          Types::UniqueEventTypeId event_type) {
    return expr_of(this->atom(*atom, event_type));
  }

  // Translates a whole (possibly compound) top-level predicate into one
  // formula, mirroring EXACTLY how it is evaluated at runtime (including which
  // children are gated on the event type, see PhysicalPredicate::translate).
  z3::expr translate_formula(const CEA::PhysicalPredicate* node,
                             Types::UniqueEventTypeId event_type,
                             bool gated = true) {
    return expr_of(node->translate(*this, event_type, gated));
  }

  // The Z3 expression a handle stands for.
  z3::expr expr_of(Handle handle) const { return terms_[handle].value; }

  // How many distinct atoms could not be modeled for at least one event type
  // (for logging/diagnostics). An atom that is opaque for two event types still
  // counts once.
  size_t opaque_atom_count() const { return opaque_atoms_.size(); }

  // Whether `atom` could not be modeled (for at least one event type).
  bool is_opaque(const CEA::PhysicalPredicate* atom) const {
    return opaque_atoms_.contains(atom);
  }

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

  // ---- CEA::FormulaBuilder: what the predicates call ------------------------

  Handle int_attribute(Types::UniqueEventTypeId event_type, size_t pos) override {
    return store(Term{attribute_symbol(event_type, pos, false), std::nullopt});
  }

  Handle double_attribute(Types::UniqueEventTypeId event_type, size_t pos) override {
    return store(Term{attribute_symbol(event_type, pos, true), nan_flag(event_type, pos)});
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
  Handle
  int_attribute_as_double(Types::UniqueEventTypeId event_type, size_t pos) override {
    z3::expr integer = attribute_symbol(event_type, pos, false);
    z3::expr max_exact = ctx_.int_val(kMaxExactInt);
    z3::expr min_exact = ctx_.int_val(-kMaxExactInt);
    z3::expr max_exact_real = z3::to_real(max_exact);
    z3::expr min_exact_real = z3::to_real(min_exact);

    z3::expr rounded = conversion_symbol(event_type, pos);
    z3::expr at_least_max = z3::ite(rounded >= max_exact_real, rounded, max_exact_real);
    z3::expr at_most_min = z3::ite(rounded <= min_exact_real, rounded, min_exact_real);
    return store(
      Term{z3::ite(integer > max_exact,
                   at_least_max,
                   z3::ite(integer < min_exact, at_most_min, z3::to_real(integer))),
           std::nullopt});
  }

  Handle int_literal(int64_t value) override {
    return store(Term{ctx_.int_val(value), std::nullopt});
  }

  // Doubles are written as the shortest decimal that round-trips, in plain
  // (non-scientific) notation so Z3's parser accepts any magnitude. That keeps
  // the ordering between distinct double literals exact. NaN/infinity cannot be
  // a Z3 real, so such literals make the atom opaque.
  std::optional<Handle> double_literal(double value) override {
    if (!std::isfinite(value)) return std::nullopt;
    char buffer[512];
    auto [end, error] = std::to_chars(buffer,
                                      buffer + sizeof(buffer),
                                      value,
                                      std::chars_format::fixed);
    if (error != std::errc()) return std::nullopt;
    return store(Term{ctx_.real_val(std::string(buffer, end).c_str()), std::nullopt});
  }

  Handle add(Handle left, Handle right) override {
    z3::expr result = terms_[left].value + terms_[right].value;
    return store(Term{result, std::nullopt});
  }

  Handle sub(Handle left, Handle right) override {
    z3::expr result = terms_[left].value - terms_[right].value;
    return store(Term{result, std::nullopt});
  }

  Handle mul(Handle left, Handle right) override {
    z3::expr result = terms_[left].value * terms_[right].value;
    return store(Term{result, std::nullopt});
  }

  // `lhs <comparison> rhs` with C++/IEEE semantics for NaN: any comparison
  // involving NaN is false, except != which is true.
  Handle compare(CEA::ComparisonType comparison, Handle left, Handle right) override {
    Term lhs = terms_[left];
    Term rhs = terms_[right];
    auto [left_value, right_value] = promote(lhs.value, rhs.value);

    z3::expr result = left_value != right_value;
    switch (comparison) {
      case CEA::ComparisonType::EQUALS:
        result = left_value == right_value;
        break;
      case CEA::ComparisonType::GREATER:
        result = left_value > right_value;
        break;
      case CEA::ComparisonType::GREATER_EQUALS:
        result = left_value >= right_value;
        break;
      case CEA::ComparisonType::LESS_EQUALS:
        result = left_value <= right_value;
        break;
      case CEA::ComparisonType::LESS:
        result = left_value < right_value;
        break;
      case CEA::ComparisonType::NOT_EQUALS:
        break;
    }

    for (const Term* term : {&lhs, &rhs}) {
      if (!term->may_be_nan.has_value()) continue;
      if (comparison == CEA::ComparisonType::NOT_EQUALS) {
        result = result || *term->may_be_nan;
      } else {
        result = result && !*term->may_be_nan;
      }
    }
    return store(Term{result, std::nullopt});
  }

  Handle constant_true() override {
    return store(Term{ctx_.bool_val(true), std::nullopt});
  }

  Handle constant_false() override {
    return store(Term{ctx_.bool_val(false), std::nullopt});
  }

  Handle conjunction(Handle left, Handle right) override {
    z3::expr result = terms_[left].value && terms_[right].value;
    return store(Term{result, std::nullopt});
  }

  Handle disjunction(Handle left, Handle right) override {
    z3::expr result = terms_[left].value || terms_[right].value;
    return store(Term{result, std::nullopt});
  }

  Handle negation(Handle operand) override {
    z3::expr result = !terms_[operand].value;
    return store(Term{result, std::nullopt});
  }

  // An atom, translated by the atom itself (PhysicalPredicate::translate_atom)
  // and remembered per (atom, event type). If it cannot be translated, or Z3
  // rejects part of it, it becomes an opaque, unconstrained boolean.
  Handle
  atom(const CEA::PhysicalPredicate& atom, Types::UniqueEventTypeId event_type) override {
    AtomKey key{&atom, event_type};
    auto cached = atom_cache_.find(key);
    if (cached != atom_cache_.end()) return cached->second;

    std::optional<Handle> translated;
    try {
      translated = atom.translate_atom(*this, event_type);
    } catch (const z3::exception&) {
      // Z3 rejected something (e.g. a malformed numeral): stay safe and opaque.
      translated = std::nullopt;
    }

    if (!translated.has_value()) opaque_atoms_.insert(&atom);
    Handle result = translated.has_value()
                      ? *translated
                      : store(Term{fresh_opaque_bool(), std::nullopt});
    atom_cache_.emplace(key, result);
    return result;
  }

 private:
  // A translated operand or formula. `may_be_nan` is only set for double
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
  // Handles index into this arena: what a FormulaBuilder::Handle stands for.
  std::vector<Term> terms_;
  std::unordered_map<AtomKey, Handle, AtomKeyHash> atom_cache_;
  std::unordered_map<AttributeKey, z3::expr, AttributeKeyHash> attribute_symbol_cache_;
  std::unordered_map<AttributeKey, z3::expr, AttributeKeyHash> nan_flag_cache_;
  std::unordered_map<AttributeKey, z3::expr, AttributeKeyHash> conversion_symbol_cache_;
  uint64_t opaque_counter_ = 0;
  std::unordered_set<const CEA::PhysicalPredicate*> opaque_atoms_;

  Handle store(Term term) {
    terms_.push_back(std::move(term));
    return terms_.size() - 1;
  }

  // A brand-new boolean about which Z3 knows nothing (see "opaque" above).
  z3::expr fresh_opaque_bool() {
    std::string name = "opaque_" + std::to_string(opaque_counter_++);
    return ctx_.bool_const(name.c_str());
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

  // Z3 needs both sides of a comparison to share a sort. Every int64_t -> double
  // conversion is made explicitly beforehand (int_attribute_as_double), so a mix
  // reaching this point would be an exact, and therefore unsound, conversion.
  // Refuse it: atom() turns the exception into an opaque atom.
  static std::pair<z3::expr, z3::expr> promote(z3::expr lhs, z3::expr rhs) {
    if (lhs.is_real() != rhs.is_real()) {
      throw z3::exception("int and real operands must be converted explicitly");
    }
    return {lhs, rhs};
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree
