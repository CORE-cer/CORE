#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
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
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/port_number.hpp"
#include "shared/networking/message_sender/zmq_message_sender.hpp"

namespace CORE::Internal::Interface::Module::Quarantine {

template <typename ResultHandlerT>
class BasePolicy {
  Catalog& catalog;
  RingTupleQueue::Queue& queue;
  std::atomic<Types::PortNumber>& next_available_inproc_port;

  using QueryVariant = std::variant<std::unique_ptr<Query::SimpleQuery<ResultHandlerT>>,
                                    std::unique_ptr<Query::PartitionByQuery<ResultHandlerT>>>;

  std::vector<QueryCatalog> query_catalogs;
  std::vector<QueryVariant> queries;
  std::vector<Internal::ZMQMessageSender> inner_thread_event_senders = {};

  std::mutex queries_lock;

 protected:
  std::vector<std::reference_wrapper<std::atomic<uint64_t*>>> last_received_tuple = {};
  std::vector<uint64_t*> last_sent_tuple = {};
  std::thread worker_thread;
  std::atomic<bool> stop_condition = false;

  std::mutex tuples_lock;
  std::vector<RingTupleQueue::Tuple> tuples;

 public:
  BasePolicy(Catalog& catalog,
             RingTupleQueue::Queue& queue,
             std::atomic<Types::PortNumber>& next_available_inproc_port)
      : catalog(catalog),
        queue(queue),
        next_available_inproc_port(next_available_inproc_port) {
    start();
  }

  BasePolicy(const BasePolicy&) = delete;
  BasePolicy& operator=(const BasePolicy&) = delete;
  BasePolicy(BasePolicy&&) = delete;
  BasePolicy& operator=(BasePolicy&&) = delete;

  virtual ~BasePolicy() {}

  void declare_query(Internal::CEQL::Query&& parsed_query,
                     std::unique_ptr<ResultHandlerT>&& result_handler) {
    std::lock_guard<std::mutex> lock(queries_lock);
    if (parsed_query.partition_by.partition_attributes.size() != 0) {
      using QueryDirectType = Query::PartitionByQuery<ResultHandlerT>;
      using QueryBaseType = Query::GenericQuery<Query::PartitionByQuery<ResultHandlerT>,
                                                ResultHandlerT>;

      initialize_query<QueryDirectType, QueryBaseType>(std::move(parsed_query),
                                                       std::move(result_handler));
    } else {
      using QueryDirectType = Query::SimpleQuery<ResultHandlerT>;
      using QueryBaseType = Query::GenericQuery<Query::SimpleQuery<ResultHandlerT>,
                                                ResultHandlerT>;

      initialize_query<QueryDirectType, QueryBaseType>(std::move(parsed_query),
                                                       std::move(result_handler));
    }
  }

  void receive_tuple(RingTupleQueue::Tuple& tuple) {
    std::lock_guard<std::mutex> lock(tuples_lock);
    tuples.push_back(tuple);
  }

 protected:
  void send_tuple_to_queries(const RingTupleQueue::Tuple& tuple) {
    ZoneScopedN("BasePolicy::send_event_to_queries");
    std::lock_guard<std::mutex> lock(queries_lock);
    uint64_t ns = tuple.nanoseconds();
    for (int i = 0; i < inner_thread_event_senders.size(); i++) {
      ZMQMessageSender& sender = inner_thread_event_senders[i];
      QueryCatalog& query_catalog = query_catalogs[i];
      if (query_catalog.is_unique_event_id_relevant_to_query(tuple.id())) {
        last_sent_tuple[i] = tuple.get_data();
        sender.send(tuple.serialize_data());
      }
    }
  }

  virtual void try_to_send_tuples() = 0;

 private:
  void start() {
    worker_thread = std::thread([&]() {
      ZoneScopedN("BasePolicy::start::worker_thread");  //NOLINT
      while (!stop_condition) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        try_to_send_tuples();
      }
    });
  }

  template <typename QueryDirectType, typename QueryBaseType>
  void initialize_query(Internal::CEQL::Query&& parsed_query,
                        std::unique_ptr<ResultHandlerT>&& result_handler) {
    std::string inproc_receiver_address = "inproc://"
                                          + std::to_string(next_available_inproc_port++);
    QueryCatalog query_catalog(catalog, parsed_query.from.streams);
    query_catalogs.push_back(query_catalog);
    queries.emplace_back(std::make_unique<QueryDirectType>(query_catalog,
                                                           queue,
                                                           inproc_receiver_address,
                                                           std::move(result_handler)));
    QueryBaseType* query = static_cast<QueryBaseType*>(
      std::get<std::unique_ptr<QueryDirectType>>(queries.back()).get());

    query->init(std::move(parsed_query));
    last_received_tuple.emplace_back(query->last_received_tuple);
    last_sent_tuple.push_back(nullptr);

    zmq::context_t& inproc_context = query->get_inproc_context();
    inner_thread_event_senders.emplace_back(inproc_receiver_address, inproc_context);
  }

  std::optional<RingTupleQueue::Tuple>
  serialized_message_to_tuple(std::string& serialized_message) {
    if (serialized_message == "STOP") {
      return {};
    }
    assert(serialized_message.size() == sizeof(uint64_t*));

    uint64_t* data;
    memcpy(&data, &serialized_message[0], sizeof(uint64_t*));
    RingTupleQueue::Tuple tuple = queue.get_tuple(data);
    return tuple;
  }
};
}  // namespace CORE::Internal::Interface::Module::Quarantine
