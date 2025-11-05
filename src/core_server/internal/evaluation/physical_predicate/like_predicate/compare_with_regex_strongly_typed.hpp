#pragma once

#include "REmatch/REmatch.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

#include "REmatch/query.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::CEA {
class CompareWithRegexStronglyTyped : public PhysicalPredicate {
 private:
  size_t pos_to_compare;
  std::string regex_string;
  REmatch::Query regex_compiled;

 public:
  CompareWithRegexStronglyTyped(uint64_t event_type_id,
                                size_t pos_to_compare,
                                std::string&& regex)
      : PhysicalPredicate(event_type_id),
        pos_to_compare(pos_to_compare),
        regex_string(regex),
        regex_compiled(std::move(REmatch::reql(regex))) {}

  ~CompareWithRegexStronglyTyped() override = default;

  bool eval(Types::EventWrapper& event) override {
    const Types::StringValue
      attribute_val = event.get_attribute_at_index<Types::StringValue>(pos_to_compare);
    return regex_compiled.check(std::string(attribute_val.val));
  }

  std::string to_string() const override {
    return "Event[" + std::to_string(pos_to_compare) + "] (regex match) "
           + regex_string.data();
  }
};
}  // namespace CORE::Internal::CEA
