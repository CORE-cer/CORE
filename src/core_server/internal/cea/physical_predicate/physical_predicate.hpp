#pragma once
#include <set>

#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"

namespace InternalCORECEA {
class PhysicalPredicate {
 public:
  virtual ~PhysicalPredicate() = default;

  std::set<uint64_t> admissible_event_types;

  bool admits_any_event_type = false;

  PhysicalPredicate(uint64_t event_type_id)
      : admissible_event_types({event_type_id}) {}

  PhysicalPredicate(std::set<uint64_t> admissible_event_types)
      : admissible_event_types(admissible_event_types) {}

  PhysicalPredicate() : admits_any_event_type(true) {}

  bool operator()(RingTupleQueue::Tuple& tuple) {
    // admits_any_event_type is second in the condition because it is rare
    // that the operator() will be used on such a type.
    if (admissible_event_types.contains(tuple.id()) || admits_any_event_type)
      return eval(tuple);
    return false;
  }

  virtual bool eval(RingTupleQueue::Tuple& tuple) = 0;

  std::string complete_info_string() const {
    std::string out = "admits any event type: "
                      + std::to_string(admits_any_event_type) + "\n"
                      + " admissible event types:";
    for (auto& event_type : admissible_event_types) {
      out += " " + std::to_string(event_type);
    }
    out += "\n repr: ";
    return out + to_string();
  }

  virtual std::string to_string() const = 0;
};
}  // namespace InternalCORECEA
