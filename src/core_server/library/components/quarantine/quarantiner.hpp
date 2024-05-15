#pragma once

#include <cstddef>
#include <map>
#include <set>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "quarantine_policies/fixed_window_quarantine_policy.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"

namespace CORE::Library::Components::Quarantine {

template <typename HandlerType>
class Quarantiner {
  using Backend = CORE::Internal::Interface::Backend<HandlerType>;

 private:
  Backend& backend;
  Internal::Catalog& catalog;
  std::vector<QuarantinePolicy::FixedWindowPolicy<HandlerType>> active_quarantines;
  std::map<Types::UniqueEventTypeId, std::size_t> unique_event_id_to_active_quarantine_idx;
  std::set<std::size_t> intercepted_queries_idx;

 public:
  Quarantiner(Backend& backend, Internal::Catalog& catalog)
      : backend(backend), catalog(catalog) {}

  void declare_quarantine(std::set<Types::StreamTypeId> streams) {
    std::size_t applicable_query_idx = backend.get_number_of_queries() - 1;
    std::size_t active_quarantine_idx = active_quarantines.size();
    active_quarantines.emplace_back(
      QuarantinePolicy::FixedWindowPolicy(backend, applicable_query_idx));
    for (const Types::StreamTypeId stream_id : streams) {
      Types::StreamInfo stream_info = catalog.get_stream_info(stream_id);
      for (const Types::EventInfo& event_info : stream_info.events_info) {
        unique_event_id_to_active_quarantine_idx[event_info.id] = active_quarantine_idx;
      }
    }
  }

  // Delete Copy constructor and assigment as that should not be done with the quarantiner
  Quarantiner(const Quarantiner&) = delete;
  Quarantiner& operator=(const Quarantiner&) = delete;

  void receive_tuple(RingTupleQueue::Tuple tuple) {
    if (auto search = unique_event_id_to_active_quarantine_idx.find(tuple.id());
        search != unique_event_id_to_active_quarantine_idx.end()) {
      std::size_t quarantine_idx = search->second;
      active_quarantines[quarantine_idx].receive_tuple(tuple);
    }
    for (size_t query_idx = 0; query_idx < backend.get_number_of_queries(); query_idx++) {
      if (!intercepted_queries_idx.contains(query_idx)) {
        backend.send_tuple_to_query(tuple, query_idx);
      }
    }
  }
};

}  // namespace CORE::Library::Components::Quarantine
