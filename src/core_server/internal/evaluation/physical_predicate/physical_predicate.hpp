#pragma once

#include <cstdint>
#include <set>
#include <string>

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
