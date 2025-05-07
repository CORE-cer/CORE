#pragma once

#include <readerwriterqueue/readerwriterqueue.h>

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <tracy/Tracy.hpp>
#include <utility>
#include <variant>
#include <vector>

#include "shared/datatypes/aliases/query_info_id.hpp"

#define QUILL_ROOT_LOGGER_ONLY
#include <quill/Quill.h>             // NOLINT
#include <quill/detail/LogMacros.h>  // NOLINT

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/modules/query/query_types/generic_query.hpp"
#include "core_server/internal/interface/modules/query/query_types/partition_by_query.hpp"
#include "core_server/internal/interface/modules/query/query_types/simple_query.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

class BasePolicy {
 protected:
  std::thread worker_thread;
  std::atomic<bool> stop_condition = false;
  std::map<Types::UniqueQueryId, std::ptrdiff_t> query_id_to_query_index;

  // Events
  std::list<moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>> blocking_event_queues;
  moodycamel::BlockingReaderWriterQueue<Types::EventWrapper> send_event_queue;

 private:
  Catalog& catalog;
  std::atomic<Types::PortNumber>& next_available_inproc_port;

  using QueryVariant = std::variant<std::unique_ptr<Query::SimpleQuery>,
                                    std::unique_ptr<Query::PartitionByQuery>>;

  std::vector<QueryCatalog> query_catalogs;
  std::vector<QueryVariant> queries;

  // TODO: Optimize
  std::mutex queries_lock;

 public:
  BasePolicy(Catalog& catalog, std::atomic<Types::PortNumber>& next_available_inproc_port)
      : catalog(catalog), next_available_inproc_port(next_available_inproc_port) {}

  BasePolicy(const BasePolicy&) = delete;
  BasePolicy& operator=(const BasePolicy&) = delete;
  BasePolicy(BasePolicy&&) = delete;
  BasePolicy& operator=(BasePolicy&&) = delete;

  virtual ~BasePolicy() {}

  void
  declare_query(Internal::CEQL::Query&& parsed_query,
                Types::UniqueQueryId query_id,
                std::unique_ptr<Library::Components::ResultHandler>&& result_handler) {
    std::lock_guard<std::mutex> lock(queries_lock);
    if (parsed_query.partition_by.partition_attributes.size() != 0) {
      using QueryDirectType = Query::PartitionByQuery;

      initialize_query<QueryDirectType>(std::move(parsed_query),
                                        std::move(query_id),
                                        std::move(result_handler));
    } else {
      using QueryDirectType = Query::SimpleQuery;

      initialize_query<QueryDirectType>(std::move(parsed_query),
                                        std::move(query_id),
                                        std::move(result_handler));
    }
  }

  virtual void receive_event(Types::EventWrapper&& event) = 0;

  void inactivate_query(Types::UniqueQueryId query_id) {
    ZoneScopedN("BasePolicy::inactivate_query");
    LOG_INFO("Inactivating query with id {} in BasePolicy::inactivate_query", query_id);
    std::lock_guard<std::mutex> lock(queries_lock);
    auto query_index_it = query_id_to_query_index.find(query_id);

    if (query_index_it == query_id_to_query_index.end()) {
      return;
    }
    std::ptrdiff_t query_index = query_index_it->second;

    // Remove query from queries
    queries.erase(queries.begin() + query_index);

    // Remove from the query_catalogs
    query_catalogs.erase(query_catalogs.begin() + query_index);

    // Remove from blocking_event_queues
    auto event_queue_it = blocking_event_queues.begin();
    std::advance(event_queue_it, query_index);
    blocking_event_queues.erase(event_queue_it);

    // Remove from query_id_to_query_index
    query_id_to_query_index.erase(query_id);

    // Fix other query indexes due to the removal
    for (auto& [id, index] : query_id_to_query_index) {
      if (index > query_index) {
        query_id_to_query_index[id] = index - 1;
      }
    }

    LOG_INFO("Query with id {} inactivated in BasePolicy::inactivate_query", query_id);
  }

 protected:
  virtual void try_add_tuples_to_send_queue() = 0;

  virtual void force_add_tuples_to_send_queue() = 0;

  void send_events_to_queries() {
    ZoneScopedN("BasePolicy::send_events_to_queries");
    LOG_L3_BACKTRACE("Sending events to queries in BasePolicy::send_events_to_queries");

    Types::EventWrapper event;
    while (send_event_queue.try_dequeue(event)) {
      LOG_L3_BACKTRACE(
        "Sending event with id {} and time to queries in "
        "BasePolicy::send_events_to_queries",
        event.get_unique_event_type_id(),
        event.get_primary_time().val);

      send_event_to_queries(std::move(event));
    }
  }

  void send_event_to_queries(Types::EventWrapper&& event) {
    ZoneScopedN("BasePolicy::send_event_to_queries");
    std::lock_guard<std::mutex> lock(queries_lock);
    int i = 0;
    for (auto& blocking_event_queue : blocking_event_queues) {
      QueryCatalog& query_catalog = query_catalogs[i];
      if (query_catalog.is_unique_event_id_relevant_to_query(
            event.get_unique_event_type_id())) {
        blocking_event_queue.enqueue(std::move(event.clone()));
      }
      i += 1;
    }
  }

  void handle_destruction() {
    stop_condition = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    for (auto& blocking_event_queue : blocking_event_queues) {
      while (blocking_event_queue.size_approx() != 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
      }
    }
    worker_thread.join();
  }

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("BasePolicy::start::worker_thread");  //NOLINT
      while (!stop_condition) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        try_add_tuples_to_send_queue();
        send_events_to_queries();
      }
      force_add_tuples_to_send_queue();
      send_events_to_queries();
    });
  }

 private:
  template <typename QueryDirectType>
  void
  initialize_query(Internal::CEQL::Query&& parsed_query,
                   Types::UniqueQueryId query_id,
                   std::unique_ptr<Library::Components::ResultHandler>&& result_handler) {
    std::string inproc_receiver_address = "inproc://"
                                          + std::to_string(next_available_inproc_port++);
    QueryCatalog query_catalog(catalog, parsed_query);

    query_catalogs.push_back(query_catalog);

    if (queries.size() > std::numeric_limits<std::ptrdiff_t>::max() - 1) {
      throw std::runtime_error("Too many queries");
    }

    std::ptrdiff_t query_index = static_cast<std::ptrdiff_t>(queries.size());
    query_id_to_query_index[query_id] = query_index;

    moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>&
      blocking_event_queue = blocking_event_queues.emplace_back(1000);
    queries.emplace_back(std::make_unique<QueryDirectType>(query_catalog,
                                                           inproc_receiver_address,
                                                           std::move(result_handler),
                                                           blocking_event_queue));
    Query::GenericQuery* query = static_cast<Query::GenericQuery*>(
      std::get<std::unique_ptr<QueryDirectType>>(queries.back()).get());

    query->init(std::move(parsed_query));
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
