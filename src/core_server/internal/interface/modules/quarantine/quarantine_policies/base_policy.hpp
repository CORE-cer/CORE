#pragma once

#include <readerwriterqueue/readerwriterqueue.h>

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstring>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <tracy/Tracy.hpp>
#include <utility>
#include <variant>
#include <vector>
#include <zmq.hpp>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/modules/query/query_types/generic_query.hpp"
#include "core_server/internal/interface/modules/query/query_types/partition_by_query.hpp"
#include "core_server/internal/interface/modules/query/query_types/simple_query.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

class BasePolicy {
  Catalog& catalog;
  std::atomic<Types::PortNumber>& next_available_inproc_port;

  using QueryVariant = std::variant<std::unique_ptr<Query::SimpleQuery>,
                                    std::unique_ptr<Query::PartitionByQuery>>;

  std::vector<QueryCatalog> query_catalogs;
  std::vector<QueryVariant> queries;
  std::vector<Internal::ZMQMessageSender> inner_thread_event_senders = {};

  // TODO: Optimize
  std::mutex queries_lock;

 protected:
  std::thread worker_thread;
  std::atomic<bool> stop_condition = false;

  // Events
  std::list<moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>> blocking_event_queues;
  moodycamel::BlockingReaderWriterQueue<Types::EventWrapper> send_event_queue;

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
                std::unique_ptr<Library::Components::ResultHandler>&& result_handler) {
    std::lock_guard<std::mutex> lock(queries_lock);
    if (parsed_query.partition_by.partition_attributes.size() != 0) {
      using QueryDirectType = Query::PartitionByQuery;

      initialize_query<QueryDirectType>(std::move(parsed_query),
                                        std::move(result_handler));
    } else {
      using QueryDirectType = Query::SimpleQuery;

      initialize_query<QueryDirectType>(std::move(parsed_query),
                                        std::move(result_handler));
    }
  }

  virtual void receive_event(Types::EventWrapper&& event) = 0;

 protected:
  virtual void try_add_tuples_to_send_queue() = 0;

  virtual void force_add_tuples_to_send_queue() = 0;

  void send_events_to_queries() {
    ZoneScopedN("BasePolicy::send_events_to_queries");
    Types::EventWrapper event;
    while (send_event_queue.try_dequeue(event)) {
      send_event_to_queries(std::move(event));
    }
  }

  void send_event_to_queries(Types::EventWrapper&& event) {
    ZoneScopedN("BasePolicy::send_event_to_queries");
    std::lock_guard<std::mutex> lock(queries_lock);
    int i = 0;
    for (auto& blocking_event_queue : blocking_event_queues) {
      ZMQMessageSender& sender = inner_thread_event_senders[i];
      QueryCatalog& query_catalog = query_catalogs[i];
      if (query_catalog.is_unique_event_id_relevant_to_query(
            event.get_unique_event_type_id())) {
        sender.send("");
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
        std::this_thread::sleep_for(std::chrono::microseconds(50));
      }
    }
    worker_thread.join();
  }

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("BasePolicy::start::worker_thread");  //NOLINT
      while (!stop_condition) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
                   std::unique_ptr<Library::Components::ResultHandler>&& result_handler) {
    std::string inproc_receiver_address = "inproc://"
                                          + std::to_string(next_available_inproc_port++);
    QueryCatalog query_catalog(catalog, parsed_query.from.streams);
    query_catalogs.push_back(query_catalog);
    moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>&
      blocking_event_queue = blocking_event_queues.emplace_back(1000);
    queries.emplace_back(std::make_unique<QueryDirectType>(query_catalog,
                                                           inproc_receiver_address,
                                                           std::move(result_handler),
                                                           blocking_event_queue));
    Query::GenericQuery* query = static_cast<Query::GenericQuery*>(
      std::get<std::unique_ptr<QueryDirectType>>(queries.back()).get());

    query->init(std::move(parsed_query));

    zmq::context_t& inproc_context = query->get_inproc_context();
    inner_thread_event_senders.emplace_back(inproc_receiver_address, inproc_context);
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
