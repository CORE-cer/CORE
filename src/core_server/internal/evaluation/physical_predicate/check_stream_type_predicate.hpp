#pragma once

#include <string>

#include "cassert"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "physical_predicate.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

class CheckStreamTypePredicate : public PhysicalPredicate {
  QueryCatalog& query_catalog;
  Types::StreamTypeId stream_type_id;

 public:
  CheckStreamTypePredicate(Types::StreamTypeId stream_type_id, QueryCatalog& query_catalog)
      : PhysicalPredicate(),
        stream_type_id(stream_type_id),
        query_catalog(query_catalog) {}

  ~CheckStreamTypePredicate() override = default;

  bool eval(RingTupleQueue::Tuple& tuple) override {
    Types::UniqueEventTypeId tuple_unique_event_id = tuple.id();

    // If errors out, probably sent a tuple that is not in the from for this query
    Types::StreamTypeId tuple_stream_type_id = query_catalog.stream_id_from_unique_event_id(
      tuple_unique_event_id);

    return tuple_stream_type_id == stream_type_id;
  }

  bool eval(Types::EventWrapper& event) override {
    Types::UniqueEventTypeId unique_event_id = event.get_unique_event_type_id();

    // If errors out, probably sent a tuple that is not in the from for this query
    Types::StreamTypeId tuple_stream_type_id = query_catalog.stream_id_from_unique_event_id(
      unique_event_id);

    return tuple_stream_type_id == stream_type_id;
  }

  std::string to_string() const override {
    std::string out = "CheckStreamTypePredicate ";
    for (auto id : admissible_event_types) {
      out += std::to_string(id) + " ";
    }
    return out;
  }
};

}  // namespace CORE::Internal::CEA
