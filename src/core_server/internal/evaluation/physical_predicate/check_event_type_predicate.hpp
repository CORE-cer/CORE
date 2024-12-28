#pragma once

#include <string>

#include "cassert"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

class CheckEventNamePredicate : public PhysicalPredicate {
  QueryCatalog& query_catalog;
  Types::EventNameTypeId event_name_type_id;

 public:
  CheckEventNamePredicate(Types::EventNameTypeId event_name_type_id,
                          QueryCatalog& query_catalog)
      : PhysicalPredicate(),
        event_name_type_id(event_name_type_id),
        query_catalog(query_catalog) {}

  ~CheckEventNamePredicate() override = default;

  bool eval(Types::EventWrapper& event) override {
    Types::UniqueEventTypeId unique_event_id = event.get_unique_event_type_id();

    Types::EventNameTypeId
      tuple_event_name_type_id = query_catalog.event_name_id_from_unique_event_id(
        unique_event_id);

    return tuple_event_name_type_id == event_name_type_id;
  }

  std::string to_string() const override {
    std::string out = "CheckEventNamePredicate ";
    for (auto id : admissible_event_types) {
      out += std::to_string(id) + " ";
    }
    return out;
  }
};

}  // namespace CORE::Internal::CEA
