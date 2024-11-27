#pragma once
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "cassert"
#include "physical_predicate.hpp"
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

  std::string to_string() const override { return "NOT " + predicate->to_string(); }
};
}  // namespace CORE::Internal::CEA
