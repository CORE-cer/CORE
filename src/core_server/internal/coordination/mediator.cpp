#include "mediator.hpp"

#include <memory>
#include <vector>

#include "core_server/internal/coordination/streams_listener.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE {
namespace Internal {

Mediator::Mediator(Types::PortNumber port)
    : router(*this, port),
      streams_listener(this, port + 1),
      current_next_port_number(port + 2),
      queue(100000, &catalog.tuple_schemas) {}

void Mediator::start() {
  router.start();
  streams_listener.start();
}

void Mediator::stop() {
  router.stop();
  streams_listener.stop();
  for (auto& complex_event_streamer : query_evaluators) {
    complex_event_streamer->stop();
  }
}

Types::PortNumber Mediator::create_complex_event_stream(std::string ecs) {
  // TODO
}

Types::PortNumber Mediator::create_dummy_complex_event_stream(
  Evaluation::PredicateEvaluator&& evaluator) {
  auto query_evaluator = std::make_unique<QueryEvaluator>(
    std::move(evaluator),
    current_next_port_number,
    current_next_port_number,
    queue);
  query_evaluators.push_back(std::move(query_evaluator));
  query_evaluators.back()->start();
  std::string address = "inproc://"
                        + std::to_string(current_next_port_number);
  zmq::context_t& shared_context = query_evaluators.back()
                                     ->get_inner_thread_context();
  inner_thread_event_senders.emplace_back(address, shared_context);
  return current_next_port_number++;
}

void Mediator::send_event_to_queries(Types::StreamTypeId stream_id,
                                     Types::Event event) {
  // TODO: send events to specific queries
  for (auto& sender : inner_thread_event_senders) {
    sender.send(event_to_tuple(event).serialize_data());
  }
}

RingTupleQueue::Tuple Mediator::event_to_tuple(Types::Event& event) {
  if (event.event_type_id > catalog.number_of_events()) {
    throw std::runtime_error("Provided event type id is not valid.");
  }
  Types::EventInfo event_info = catalog.get_event_info(event.event_type_id);
  std::vector<Types::AttributeInfo> attr_infos = event_info.attributes_info;
  if (attr_infos.size() != event.attributes.size()) {
    throw std::runtime_error("Event had an incorrect number of attributes");
  }

  uint64_t* data = queue.start_tuple(event.event_type_id);

  for (size_t i = 0; i < attr_infos.size(); i++) {
    auto& attr_info = attr_infos[i];
    std::shared_ptr<Types::Value> attr = event.attributes[i];
    switch (attr_info.value_type) {
      case Types::INT64:
        write_int(attr);
        break;
      case Types::DOUBLE:
        write_double(attr);
        break;
      case Types::BOOL:
        write_bool(attr);
        break;
      case Types::STRING_VIEW:
        write_string_view(attr);
        break;
      case Types::DATE:
        write_date(attr);
        break;
      default:
        assert(false
               && "A value type was added but not updated in the switch in event_to_tuple");
    }
  }
  return queue.get_tuple(data);
}

void Mediator::write_int(std::shared_ptr<Types::Value>& attr) {
  Types::IntValue* val_ptr = dynamic_cast<Types::IntValue*>(attr.get());
  if (val_ptr == nullptr)
    throw std::runtime_error(
      "An attribute type that is not an IntValue was provided where it "
      "should have been an IntValue!");
  int64_t* integer_ptr = queue.writer<int64_t>();
  *integer_ptr = val_ptr->val;
}

void Mediator::write_double(std::shared_ptr<Types::Value>& attr) {
  Types::DoubleValue* val_ptr = dynamic_cast<Types::DoubleValue*>(
    attr.get());
  if (val_ptr == nullptr)
    throw std::runtime_error(
      "An attribute type that is not an DoubleValue was provided where it "
      "should have been an DoubleValue!");
  double* double_ptr = queue.writer<double>();
  *double_ptr = val_ptr->val;
}

void Mediator::write_bool(std::shared_ptr<Types::Value>& attr) {
  Types::BoolValue* val_ptr = dynamic_cast<Types::BoolValue*>(attr.get());
  if (val_ptr == nullptr)
    throw std::runtime_error(
      "An attribute type that is not an BoolValue was provided where it "
      "should have been an BoolValue!");
  bool* bool_ptr = queue.writer<bool>();
  *bool_ptr = val_ptr->val;
}

void Mediator::write_string_view(std::shared_ptr<Types::Value>& attr) {
  Types::StringValue* val_ptr = dynamic_cast<Types::StringValue*>(
    attr.get());
  if (val_ptr == nullptr)
    throw std::runtime_error(
      "An attribute type that is not a StringValue was provided where it "
      "should have been a StringValue!");
  char* chars = queue.writer<std::string>(val_ptr->val.size());
  memcpy(chars, &val_ptr->val[0], val_ptr->val.size());
}

void Mediator::write_date(std::shared_ptr<Types::Value>& attr) {
  Types::DateValue* val_ptr = dynamic_cast<Types::DateValue*>(attr.get());
  if (val_ptr == nullptr)
    throw std::runtime_error(
      "An attribute type that is not a DateValue was provided where it "
      "should have been a DateValue!");
  std::time_t* time_ptr = queue.writer<std::time_t>();
  *time_ptr = val_ptr->val;
}
}  // namespace Internal
}  // namespace CORE
