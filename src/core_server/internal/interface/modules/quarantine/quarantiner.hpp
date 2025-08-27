#pragma once

#include <quill/Frontend.h>
#include <quill/LogMacros.h>
#include <quill/Logger.h>

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <tracy/Tracy.hpp>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/base_policy.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/direct_policy.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/quarantine_policy_type.hpp"
#include "core_server/internal/interface/modules/quarantine/quarantine_policies/wait_fixed_time_policy.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/aliases/query_info_id.hpp"
#include "shared/datatypes/aliases/stream_type_id.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

class QuarantineManager {
  quill::Logger* logger = quill::Frontend::get_logger("root");
  Catalog& catalog;

  std::atomic<Types::PortNumber> next_available_inproc_port{5000};

  std::map<std::set<Types::StreamTypeId>, std::unique_ptr<BasePolicy>> query_policies;

  std::map<Types::StreamTypeId, std::vector<std::reference_wrapper<BasePolicy>>>
    stream_type_id_to_relevant_policies;

 public:
  QuarantineManager(Catalog& catalog) : catalog(catalog) {}

  void
  declare_query(Internal::CEQL::Query&& parsed_query,
                std::string query_name,
                std::unique_ptr<Library::Components::ResultHandler>&& result_handler) {
    std::set<Types::StreamTypeId> stream_type_ids = get_stream_ids_from_names(
      parsed_query.from.streams);

    auto iter = query_policies.find(stream_type_ids);

    // Check if set of streams already has policy associated with it
    if (iter != query_policies.end()) {
      BasePolicy& query_policy = *(iter->second);
      Types::UniqueQueryId query_id = catalog.add_query(
        {result_handler->get_identifier(),
         result_handler->get_result_handler_type(),
         parsed_query.to_string(),
         query_name,
         parsed_query.select.attribute_projection_variable,
         parsed_query.select.attribute_projection_stream_event});
      query_policy.declare_query(std::move(parsed_query),
                                 std::move(query_id),
                                 std::move(result_handler));
    } else {
      QuarantinePolicy quarantine_policy =
        {QuarantinePolicy::QuarantinePolicyType::DirectPolicy, parsed_query.from.streams};
      set_query_policy(std::move(quarantine_policy));
      declare_query(std::move(parsed_query),
                    std::move(query_name),
                    std::move(result_handler));
    }
  }

  void inactivate_query(Types::UniqueQueryId query_id) {
    LOG_INFO(logger,
             "Inactivating query with id {} in QuarantineManager::inactivate_query",
             query_id);
    for (auto& [stream_type_ids, query_policy] : query_policies) {
      query_policy->inactivate_query(query_id);
    }

    catalog.inactivate_query(query_id);
  }

  void send_event_to_queries(Types::StreamTypeId stream_id,
                             const Types::EventWrapper&& event) {
    ZoneScopedN("QuarantineManager::send_event_to_queries");
    LOG_TRACE_L3(logger,
                 "Received event with id {} from stream with id {} in "
                 "QuarantineManager::send_event_to_queries",
                 event.get_unique_event_type_id(),
                 stream_id);
    std::vector<std::reference_wrapper<BasePolicy>>&
      relevant_policies = stream_type_id_to_relevant_policies[stream_id];

    for (BasePolicy& relevant_policy : relevant_policies) {
      relevant_policy.receive_event(std::move(event.clone()));
    }
  }

  void set_query_policy(Module::Quarantine::QuarantinePolicy&& quarantine_policy) {
    std::unique_ptr<BasePolicy> query_policy_ptr = std::move(
      create_policy(quarantine_policy));
    std::set<Types::StreamTypeId> stream_type_ids = get_stream_ids_from_names(
      quarantine_policy.streams);

    auto iter = query_policies.insert({stream_type_ids, std::move(query_policy_ptr)});

    BasePolicy& query_policy = *(iter.first->second);
    for (const Types::StreamTypeId stream_type_id : stream_type_ids) {
      std::vector<std::reference_wrapper<BasePolicy>>&
        relevant_policies = stream_type_id_to_relevant_policies[stream_type_id];

      relevant_policies.emplace_back(query_policy);
    }
  }

 private:
  std::unique_ptr<BasePolicy> create_policy(QuarantinePolicy quarantine_policy) {
    switch (quarantine_policy.policy_type) {
      case QuarantinePolicy::QuarantinePolicyType::DirectPolicy:
        return std::make_unique<DirectPolicy>(catalog, next_available_inproc_port);
      case QuarantinePolicy::QuarantinePolicyType::WaitFixedTimePolicy:
        if (!quarantine_policy.time_window.has_value()) {
          throw std::runtime_error(
            "Time window must be specified for WaitFixedTimePolicy");
        }
        return std::make_unique<WaitFixedTimePolicy>(catalog,
                                                     next_available_inproc_port,
                                                     quarantine_policy.time_window.value());
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
