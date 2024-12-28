#pragma once
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "cassert"
#include "physical_predicate.hpp"
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

  std::string to_string() const override {
    std::string out = predicates[0]->to_string();
    for (int i = 1; i < predicates.size(); i++) {
      out += " AND " + predicates[i]->to_string();
    }
    return out;
  }
};
}  // namespace CORE::Internal::CEA
