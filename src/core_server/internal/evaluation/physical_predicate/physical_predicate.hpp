#pragma once

#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

class PhysicalPredicate {
 public:
  virtual ~PhysicalPredicate() = default;

  std::set<Types::UniqueEventTypeId> admissible_event_types;

  bool admits_any_event_type = false;

  PhysicalPredicate(Types::UniqueEventTypeId event_type_id)
      : admissible_event_types({event_type_id}) {}

  PhysicalPredicate(std::set<uint64_t> admissible_event_types)
      : admissible_event_types(admissible_event_types) {}

  PhysicalPredicate() : admits_any_event_type(true) {}

  bool operator()(Types::EventWrapper& event) {
    // admits_any_event_type is second in the condition because it is rare
    // that the operator() will be used on such a type.
    Types::UniqueEventTypeId unique_event_id = event.get_unique_event_type_id();
    if (admissible_event_types.contains(unique_event_id) || admits_any_event_type) {
      bool event_eval = eval(event);
      return event_eval;
    }
    return false;
  }

  virtual bool eval(Types::EventWrapper& event_wrapper) = 0;

  // Distinguishes compound boolean-combinator nodes (And/Or/Not) from atomic
  // leaf predicates, for traversals that need to reach the atoms (e.g. the
  // minterm-tree optimization) without a full visitor per templated leaf type.
  virtual bool is_compound() const { return false; }

  virtual std::vector<PhysicalPredicate*> get_children() const { return {}; }

  // ---- Translation to a logical formula ------------------------------------
  // Used by the optional minterm-tree optimization to reason about how
  // predicates relate to each other. None of this is used when evaluating
  // events, and it does not depend on any solver: the formula is built through
  // the abstract FormulaBuilder.

  // The formula that is true exactly when evaluating this predicate on an event
  // of `event_type` returns true. It mirrors how the predicate is evaluated:
  //   - `gated` says whether evaluation goes through operator(), which returns
  //     false for event types this predicate does not admit. The top-level
  //     predicate and the children of an Or are gated; the children of an
  //     And/Not are evaluated with eval() and are not. Getting this wrong would
  //     make the formula true for events the baseline evaluates to false.
  FormulaBuilder::Handle translate(FormulaBuilder& builder,
                                   Types::UniqueEventTypeId event_type,
                                   bool gated = true) const {
    if (gated && !admits_any_event_type && !admissible_event_types.contains(event_type)) {
      return builder.constant_false();
    }
    return translate_ungated(builder, event_type);
  }

  // The formula for evaluating this predicate directly with eval(). Compound
  // predicates (And/Or/Not) combine their children's formulas; every other
  // predicate is an atom, which the builder resolves through translate_atom.
  virtual FormulaBuilder::Handle
  translate_ungated(FormulaBuilder& builder, Types::UniqueEventTypeId event_type) const {
    return builder.atom(*this, event_type);
  }

  // An atom's own formula, or nullopt if it cannot be described exactly (regex,
  // strings, unsupported arithmetic, ...). Such an atom stays "opaque": it is
  // still evaluated natively at runtime, the solver just knows nothing about it.
  // Called by the builder from FormulaBuilder::atom.
  virtual std::optional<FormulaBuilder::Handle>
  translate_atom(FormulaBuilder& /*builder*/,
                 Types::UniqueEventTypeId /*event_type*/) const {
    return std::nullopt;
  }

  std::string complete_info_string() const {
    std::string out = "admits any event type: " + std::to_string(admits_any_event_type)
                      + "\n" + " admissible event types:";
    for (auto& event_type : admissible_event_types) {
      out += " " + std::to_string(event_type);
    }
    out += "\n repr: ";
    return out + to_string();
  }

  virtual std::string to_string() const = 0;
};
}  // namespace CORE::Internal::CEA
