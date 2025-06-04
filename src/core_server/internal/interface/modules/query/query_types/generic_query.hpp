#pragma once

#include <readerwriterqueue/readerwriterqueue.h>

#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <tracy/Tracy.hpp>
#include <utility>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::Interface::Module::Query {
class GenericQuery {
 protected:
  uint64_t current_stream_position = 0;
  Internal::QueryCatalog query_catalog;
  std::unique_ptr<Library::Components::ResultHandler> result_handler;

  // Receiver for tuples
  std::atomic<bool> stop_condition = false;
  std::thread worker_thread;

  // Events
  moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>& blocking_event_queue;

 public:
  std::atomic<uint64_t> time_of_expiration = 0;
  CEQL::Within::TimeWindow time_window;

  GenericQuery(
    Internal::QueryCatalog query_catalog,
    std::string inproc_receiver_address,
    std::unique_ptr<Library::Components::ResultHandler>&& result_handler,
    moodycamel::BlockingReaderWriterQueue<Types::EventWrapper>& blocking_event_queue)
      : query_catalog(query_catalog),
        result_handler(std::move(result_handler)),
        blocking_event_queue(blocking_event_queue) {}

  void init(Internal::CEQL::Query&& query) {
    create_query(std::move(query));
    start();
  }

  virtual ~GenericQuery() {};

  Library::Components::ResultHandler& get_result_handler_reference() const {
    return *result_handler;
  }

 protected:
  void stop() {
    try {
      stop_condition = true;
      worker_thread.join();
    } catch (std::exception& e) {
      std::cout << "Exception: " << e.what() << std::endl;
    }
  }

 private:
  virtual void create_query(Internal::CEQL::Query&& query) = 0;

  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("QueryImpl::start::worker_thread");  //NOLINT
      result_handler->set_query_catalog(query_catalog);
      result_handler->start();
      while (!stop_condition) {
        std::optional<Types::EventWrapper> event = get_event();
        if (!event.has_value()) {
          continue;
        }
        std::optional<tECS::Enumerator> output = process_event(std::move(event.value()));
        (*result_handler)(std::move(output));
      }
    });
  }

  std::optional<Types::EventWrapper> get_event() {
    Types::EventWrapper event;
    bool got_event = blocking_event_queue.wait_dequeue_timed(event,
                                                             std::chrono::milliseconds(50));
    if (!got_event) {
      return {};
    }
    return std::move(event);
  }

  virtual std::optional<tECS::Enumerator> process_event(Types::EventWrapper&& event) = 0;
};
}  // namespace CORE::Internal::Interface::Module::Query
