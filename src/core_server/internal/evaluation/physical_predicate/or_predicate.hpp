#pragma once
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "cassert"
#include "physical_predicate.hpp"
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

  std::string to_string() const override {
    std::string out = predicates[0]->to_string();
    for (int i = 1; i < predicates.size(); i++) {
      out += " OR " + predicates[i]->to_string();
    }
    return out;
  }
};
}  // namespace CORE::Internal::CEA
