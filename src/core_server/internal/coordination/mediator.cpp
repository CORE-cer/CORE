#include "mediator.hpp"

#include <memory>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/streams_listener.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE::Internal {

Mediator::Mediator(Types::PortNumber port, uint64_t default_time_duration)
    : router(*this, port),
      streams_listener(this, port + 1),
      current_next_port_number(port + 2),
      queue(100000, &catalog.tuple_schemas),
      default_time_duration(default_time_duration) {}

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

Types::PortNumber Mediator::create_complex_event_stream(CEQL::Query query) {
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = Evaluation::PredicateEvaluator(
    std::move(predicates));

  auto visitor = CEQL::FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  CEA::DetCEA cea(CEA::CEA(std::move(visitor.current_cea)));
  uint64_t time_window = query.within.time_window.mode
                             == CEQL::Within::TimeWindowMode::NONE
                           ? default_time_duration
                           : query.within.time_window.duration;

  time_is_event_based.push_back(query.within.time_window.mode
                                == CEQL::Within::TimeWindowMode::EVENTS);

  query_events_expiration_time.push_back(std::make_unique<uint64_t>(0));
  auto evaluator = std::make_unique<Evaluation::Evaluator>(
    std::move(cea),
    std::move(tuple_evaluator),
    time_window,
    *query_events_expiration_time.back());
  query_evaluators.emplace_back(std::make_unique<QueryEvaluator>(
    std::move(evaluator),
    query.within.time_window.mode
      != CEQL::Within::TimeWindowMode::NANOSECONDS,
    current_next_port_number,
    current_next_port_number,
    queue,
    catalog));
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
  RingTupleQueue::Tuple tuple = event_to_tuple(event);
  uint64_t ns = tuple.nanoseconds();
  if (!previous_event_sent) {
    previous_event_sent = ns;
  }
  maximum_historic_time_between_events = std::max(
    maximum_historic_time_between_events, ns - previous_event_sent.value());
  previous_event_sent = ns;
  // TODO: send events to specific queries that require it.
  for (auto& sender : inner_thread_event_senders) {
    sender.send(tuple.serialize_data());
  }

  // TODO: Don't do this always.
  update_space_of_ring_tuple_queue();
}

void Mediator::update_space_of_ring_tuple_queue() {
  if (query_events_expiration_time.size() != 0) {
    assert(query_events_expiration_time.size()
           == time_is_event_based.size());
    uint64_t consensus = UINT64_MAX;
    for (size_t i = 0; i < time_is_event_based.size(); i++) {
      if (time_is_event_based[i]) {
        consensus = std::min(*query_events_expiration_time[i]
                               * maximum_historic_time_between_events,
                             consensus);
      } else {
        consensus = std::min(*query_events_expiration_time[i], consensus);
      }
    }
    queue.update_overwrite_timepoint(consensus);
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

}  // namespace CORE::Internal
