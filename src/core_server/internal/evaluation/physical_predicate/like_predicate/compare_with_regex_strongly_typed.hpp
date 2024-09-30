#pragma once

#include <re2/re2.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::CEA {
class CompareWithRegexStronglyTyped : public PhysicalPredicate {
 private:
  size_t pos_to_compare;
  std::string regex_string;
  re2::RE2 regex_compiled;

 public:
  CompareWithRegexStronglyTyped(uint64_t event_type_id,
                                size_t pos_to_compare,
                                std::string&& regex)
      : PhysicalPredicate(event_type_id),
        pos_to_compare(pos_to_compare),
        regex_string(regex),
        regex_compiled(regex) {}

  ~CompareWithRegexStronglyTyped() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    uint64_t* pos = tuple[pos_to_compare];
    RingTupleQueue::Value<std::string_view> attribute_val(pos);
    return re2::RE2::FullMatch(attribute_val.get(), regex_compiled);
  }

  bool eval(Types::EventWrapper& event) override {
    const Types::StringValue attribute_val = event.get_attribute_at_index<Types::StringValue>(pos_to_compare);
    return re2::RE2::FullMatch(attribute_val.val, regex_compiled);
  }

  std::string to_string() const override {
    return "Event[" + std::to_string(pos_to_compare) + "] (regex match) "
           + regex_string.data();
  }
};
}  // namespace CORE::Internal::CEA
