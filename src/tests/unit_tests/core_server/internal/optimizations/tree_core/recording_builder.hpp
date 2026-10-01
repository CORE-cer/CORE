#pragma once

// A FormulaBuilder that writes the formula as text instead of building a solver
// expression. It lets the translate methods of the physical predicates be tested
// without Z3: a handle is an index into a list of strings, and every builder
// call produces the string a reader would expect.
//
//   attribute 1 of event type 0, as int / as double   i(0,1)   d(0,1)
//   an int attribute converted to double               toDouble(i(0,1))
//   a string attribute / a string literal               s(0,1)   str(abc)
//   comparison, arithmetic                              (> a b)  (+ a b)  (* a b)
//   and / or / not / constants                          (and a b)  (or a b)  (not a)  true  false
//   an atom that could not be translated                opaque[<atom text>]@<event type>
//
// so `Integer1 > 5` on event type 0 reads `(> i(0,1) 5)`. String attributes/
// literals are only produced when constructed with model_string_equality=true
// (default false, matching how string equality stays opaque by default).

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

class RecordingBuilder : public CEA::FormulaBuilder {
 public:
  // `model_string_equality`: whether this builder should model string
  // equality/inequality (as it would under
  // StringEqualityStrategy::InternedEquality) or stay opaque for it (the
  // default, matching every existing test that constructs a bare
  // RecordingBuilder and expects string atoms to stay opaque).
  explicit RecordingBuilder(bool model_string_equality = false)
      : model_string_equality_(model_string_equality) {}

  // The text a handle stands for.
  const std::string& text(Handle handle) const { return formulas_[handle]; }

  // How many atoms were reported as opaque (could not be translated).
  size_t opaque_count() const { return opaque_count_; }

  Handle int_attribute(Types::UniqueEventTypeId event_type, size_t pos) override {
    return record("i(" + std::to_string(event_type) + "," + std::to_string(pos) + ")");
  }

  Handle double_attribute(Types::UniqueEventTypeId event_type, size_t pos) override {
    return record("d(" + std::to_string(event_type) + "," + std::to_string(pos) + ")");
  }

  Handle
  int_attribute_as_double(Types::UniqueEventTypeId event_type, size_t pos) override {
    return record("toDouble(i(" + std::to_string(event_type) + "," + std::to_string(pos)
                  + "))");
  }

  Handle int_literal(int64_t value) override { return record(std::to_string(value)); }

  std::optional<Handle> double_literal(double value) override {
    if (!std::isfinite(value)) return std::nullopt;
    std::ostringstream out;
    out << value;
    return record(out.str());
  }

  // No interning needed here: this test double only shows what was asked for,
  // it does not need to prove cross-atom relationships the way the real Z3
  // translator's identical-id caching does.
  Handle string_attribute(Types::UniqueEventTypeId event_type, size_t pos) override {
    return record("s(" + std::to_string(event_type) + "," + std::to_string(pos) + ")");
  }

  Handle string_literal(std::string_view value) override {
    return record("str(" + std::string(value) + ")");
  }

  bool models_string_equality() const override { return model_string_equality_; }

  Handle add(Handle left, Handle right) override { return binary("+", left, right); }

  Handle sub(Handle left, Handle right) override { return binary("-", left, right); }

  Handle mul(Handle left, Handle right) override { return binary("*", left, right); }

  Handle compare(CEA::ComparisonType comparison, Handle left, Handle right) override {
    switch (comparison) {
      case CEA::ComparisonType::EQUALS:
        return binary("==", left, right);
      case CEA::ComparisonType::GREATER:
        return binary(">", left, right);
      case CEA::ComparisonType::GREATER_EQUALS:
        return binary(">=", left, right);
      case CEA::ComparisonType::LESS_EQUALS:
        return binary("<=", left, right);
      case CEA::ComparisonType::LESS:
        return binary("<", left, right);
      case CEA::ComparisonType::NOT_EQUALS:
        break;
    }
    return binary("!=", left, right);
  }

  Handle constant_true() override { return record("true"); }

  Handle constant_false() override { return record("false"); }

  Handle conjunction(Handle left, Handle right) override {
    return binary("and", left, right);
  }

  Handle disjunction(Handle left, Handle right) override {
    return binary("or", left, right);
  }

  Handle negation(Handle operand) override {
    return record("(not " + formulas_[operand] + ")");
  }

  Handle
  atom(const CEA::PhysicalPredicate& atom, Types::UniqueEventTypeId event_type) override {
    std::optional<Handle> translated = atom.translate_atom(*this, event_type);
    if (translated.has_value()) return *translated;
    opaque_count_++;
    return record("opaque[" + atom.to_string() + "]@" + std::to_string(event_type));
  }

 private:
  bool model_string_equality_;
  std::vector<std::string> formulas_;
  size_t opaque_count_ = 0;

  Handle record(std::string formula) {
    formulas_.push_back(std::move(formula));
    return formulas_.size() - 1;
  }

  Handle binary(const std::string& op, Handle left, Handle right) {
    return record("(" + op + " " + formulas_[left] + " " + formulas_[right] + ")");
  }
};

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
