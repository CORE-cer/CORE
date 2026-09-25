#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "cassert"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

class AndPredicate : public PhysicalPredicate {
 private:
  std::vector<std::unique_ptr<PhysicalPredicate>> predicates;

 public:
  AndPredicate(uint64_t event_type_id,
               std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(event_type_id), predicates(std::move(predicates)) {}

  AndPredicate(std::set<uint64_t> admissible_event_types,
               std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(admissible_event_types), predicates(std::move(predicates)) {}

  AndPredicate(std::vector<std::unique_ptr<PhysicalPredicate>>&& predicates)
      : PhysicalPredicate(), predicates(std::move(predicates)) {}

  ~AndPredicate() override = default;

  bool eval(Types::EventWrapper& event) override {
    ZoneScopedN("AndPredicate::eval()");
    for (auto& predicate : predicates) {
      if (!(predicate->eval(event))) {
        return false;
      }
    }
    return true;
  }

  // The conjunction of the children. And evaluates them with eval(), which does not
  // check event types, so they are translated ungated.
  FormulaBuilder::Handle
  translate_ungated(FormulaBuilder& builder,
                    Types::UniqueEventTypeId event_type) const override {
    std::optional<FormulaBuilder::Handle> combined;
    for (const auto& predicate : predicates) {
      FormulaBuilder::Handle formula = predicate->translate(builder,
                                                            event_type,
                                                            /*gated=*/false);
      combined = combined.has_value() ? builder.conjunction(*combined, formula) : formula;
    }
    return combined.has_value() ? *combined : builder.constant_true();
  }

  std::string to_string() const override {
    std::string out = predicates[0]->to_string();
    for (int i = 1; i < predicates.size(); i++) {
      out += " AND " + predicates[i]->to_string();
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
