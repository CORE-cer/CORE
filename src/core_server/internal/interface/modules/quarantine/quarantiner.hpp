#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/base_policy.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/direct_policy.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/quarantine_policy_type.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/wait_fixed_time_policy.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

template <typename ResultHandlerT>
class QuarantineManager {
  Catalog& catalog;
  RingTupleQueue::Queue& queue;

  std::atomic<Types::PortNumber> next_available_inproc_port{5000};

  std::map<std::set<Types::StreamTypeId>, std::unique_ptr<BasePolicy<ResultHandlerT>>>
    query_policies;

  std::map<Types::StreamTypeId,
           std::vector<std::reference_wrapper<BasePolicy<ResultHandlerT>>>>
    stream_type_id_to_relevant_policies;

 public:
  QuarantineManager(Catalog& catalog, RingTupleQueue::Queue& queue)
      : catalog(catalog), queue(queue) {}

  void declare_query(Internal::CEQL::Query&& parsed_query,
                     std::unique_ptr<ResultHandlerT>&& result_handler) {
    std::set<Types::StreamTypeId> stream_type_ids = get_stream_ids_from_names(
      parsed_query.from.streams);

    auto iter = query_policies.find(stream_type_ids);

    // Check if set of streams already has policy associated with it
    if (iter != query_policies.end()) {
      BasePolicy<ResultHandlerT>& query_policy = *(iter->second);
      query_policy.declare_query(std::move(parsed_query), std::move(result_handler));
    } else {
      set_query_policy(std::move(stream_type_ids),
                       QuarantinePolicyType::WaitFixedTimePolicy);
      declare_query(std::move(parsed_query), std::move(result_handler));
    }
  }

  void send_tuple_to_queries(Types::StreamTypeId stream_id, RingTupleQueue::Tuple& tuple) {
    std::vector<std::reference_wrapper<BasePolicy<ResultHandlerT>>>&
      relevant_policies = stream_type_id_to_relevant_policies[stream_id];

    for (BasePolicy<ResultHandlerT>& relevant_policy : relevant_policies) {
      relevant_policy.receive_tuple(tuple);
    }
  }

  void set_query_policy(std::set<Types::StreamTypeId>&& stream_ids,
                        QuarantinePolicyType policy_type) {
    std::unique_ptr<BasePolicy<ResultHandlerT>> query_policy_ptr = std::move(
      create_policy(policy_type));

    auto iter = query_policies.insert({stream_ids, std::move(query_policy_ptr)});

    BasePolicy<ResultHandlerT>& query_policy = *(iter.first->second);
    for (const Types::StreamTypeId stream_type_id : stream_ids) {
      std::vector<std::reference_wrapper<BasePolicy<ResultHandlerT>>>&
        relevant_policies = stream_type_id_to_relevant_policies[stream_type_id];

      relevant_policies.emplace_back(query_policy);
    }
  }

 private:
  std::unique_ptr<BasePolicy<ResultHandlerT>>
  create_policy(QuarantinePolicyType policy_type) {
    switch (policy_type) {
      case QuarantinePolicyType::DirectPolicy:
        return std::make_unique<DirectPolicy<ResultHandlerT>>(catalog,
                                                              queue,
                                                              next_available_inproc_port);
      case QuarantinePolicyType::WaitFixedTimePolicy:
        return std::make_unique<WaitFixedTimePolicy<ResultHandlerT>>(
          catalog, queue, next_available_inproc_port);
      default:
        throw std::runtime_error("Invalid policy type");
    }
  }

  std::set<Types::StreamTypeId>
  get_stream_ids_from_names(std::set<std::string>& stream_names) {
    std::set<Types::StreamTypeId> stream_type_ids = {};
    for (const Types::StreamInfo& stream_info : catalog.get_all_streams_info()) {
      if (stream_names.contains(stream_info.name)) {
        stream_type_ids.insert(stream_info.id);
      }
    }
    return stream_type_ids;
  }
};

}  // namespace CORE::Internal::Interface::Module::Quarantine
