#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "cassert"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

class OrPredicate : public PhysicalPredicate {
 private:
  std::vector<std::unique_ptr<PhysicalPredicate>> predicates;

 public:
  OrPredicate(uint64_t event_type_id,
              std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(event_type_id), predicates(std::move(predicates)) {}

  OrPredicate(std::set<uint64_t> admissible_event_types,
              std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(admissible_event_types), predicates(std::move(predicates)) {}

  OrPredicate(std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(), predicates(std::move(predicates)) {}

  ~OrPredicate() override = default;

  bool eval(Types::EventWrapper& event) override {
    for (auto& predicate : predicates) {
      // We want to check for event_types individually inside the or.
      if ((*predicate)(event)) {
        return true;
      }
    }
    return false;
  }

  // The disjunction of the children. Or evaluates them through operator() ("We want to
  // check event_types individually inside the or"), so they are translated gated.
  FormulaBuilder::Handle
  translate_ungated(FormulaBuilder& builder,
                    Types::UniqueEventTypeId event_type) const override {
    std::optional<FormulaBuilder::Handle> combined;
    for (const auto& predicate : predicates) {
      FormulaBuilder::Handle formula = predicate->translate(builder,
                                                            event_type,
                                                            /*gated=*/true);
      combined = combined.has_value() ? builder.disjunction(*combined, formula) : formula;
    }
    return combined.has_value() ? *combined : builder.constant_false();
  }

  std::string to_string() const override {
    std::string out = predicates[0]->to_string();
    for (int i = 1; i < predicates.size(); i++) {
      out += " OR " + predicates[i]->to_string();
    }
    return out;
  }

  bool is_compound() const override { return true; }

  std::vector<PhysicalPredicate*> get_children() const override {
    std::vector<PhysicalPredicate*> children;
    children.reserve(predicates.size());
    for (auto& predicate : predicates) {
      children.push_back(predicate.get());
    }
    return children;
  }
};
}  // namespace CORE::Internal::CEA
