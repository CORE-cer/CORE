#pragma once
#include <cstdint>
#include <memory>
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

class NotPredicate : public PhysicalPredicate {
 private:
  std::unique_ptr<PhysicalPredicate> predicate;

 public:
  NotPredicate(uint64_t event_type_id, std::unique_ptr<PhysicalPredicate> predicate)
      : PhysicalPredicate(event_type_id), predicate(std::move(predicate)) {}

  NotPredicate(std::set<uint64_t> admissible_event_types,
               std::unique_ptr<PhysicalPredicate> predicate)
      : PhysicalPredicate(admissible_event_types), predicate(std::move(predicate)) {}

  NotPredicate(std::unique_ptr<PhysicalPredicate> predicate)
      : PhysicalPredicate(), predicate(std::move(predicate)) {}

  ~NotPredicate() override = default;

  bool eval(Types::EventWrapper& event) override { return !predicate->eval(event); }

  // The negation of the child. Not evaluates it with eval(), which does not check
  // event types, so it is translated ungated.
  FormulaBuilder::Handle
  translate_ungated(FormulaBuilder& builder,
                    Types::UniqueEventTypeId event_type) const override {
    return builder.negation(predicate->translate(builder, event_type, /*gated=*/false));
  }

  std::string to_string() const override { return "NOT " + predicate->to_string(); }

  bool is_compound() const override { return true; }

  std::vector<PhysicalPredicate*> get_children() const override {
    return {predicate.get()};
  }
};
}  // namespace CORE::Internal::CEA
