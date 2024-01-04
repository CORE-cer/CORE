#include <memory>
#include <vector>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/coordination/query_evaluator.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "shared/serializer/cereal_serializer.hpp"
#include "core_server/internal/coordination/cea_factory.hpp"

namespace CORE::Internal {

class OfflineSingleMediator {
  Catalog catalog = {};
  RingTupleQueue::Queue queue;
  CEAFactory cea_factory;
  uint64_t default_time_duration;
  std::optional<uint64_t> previous_event_sent;
  uint64_t maximum_historic_time_between_events = 0;
  std::vector<std::unique_ptr<uint64_t>> query_events_expiration_time = {};
  std::vector<bool> time_is_event_based = {};
  std::unique_ptr<QueryEvaluator> query_evaluator;

 public:
  OfflineSingleMediator(uint64_t default_time_duration = 20)
      : queue(100000, &catalog.tuple_schemas),
        default_time_duration(default_time_duration),
        cea_factory{catalog, default_time_duration} {}

  Types::Enumerator convert_enumerator(tECS::Enumerator&& enumerator) {
    return catalog.convert_enumerator(std::move(enumerator));
  }

  Types::EventTypeId
  add_event_type(std::string event_name,
                 std::vector<Types::AttributeInfo>&& event_attributes) {
    return catalog.add_event_type(std::move(event_name),
                                  std::move(event_attributes));
  }

  Types::StreamTypeId
  add_stream_type(std::string stream_name,
                  std::vector<Types::EventTypeId>&& stream_event_types) {
    return catalog.add_stream_type(std::move(stream_name),
                                   std::move(stream_event_types));
  }

  void add_query(CEQL::Query&& query) {
    CEAFactoryProduct product = cea_factory.create(std::move(query));

    time_is_event_based.push_back(product.query.within.time_window.mode
                                  == CEQL::Within::TimeWindowMode::EVENTS);

    query_events_expiration_time.push_back(std::make_unique<uint64_t>(0));
    auto evaluator = std::make_unique<Evaluation::Evaluator>(
      std::move(product.cea),
      std::move(product.tuple_evaluator),
      product.time_window,
      *query_events_expiration_time.back());

    query_evaluator = std::make_unique<QueryEvaluator>(
      std::move(evaluator),
      product.query.within.time_window.mode,
      queue,
      catalog);
  }

  // Es necesario tener stream_id??
  bool
  send_event_to_query(Types::StreamTypeId stream_id, Types::Event event) {
    RingTupleQueue::Tuple tuple = event_to_tuple(event);
    uint64_t ns = tuple.nanoseconds();
    if (!previous_event_sent) {
      previous_event_sent = ns;
    }
    maximum_historic_time_between_events = std::max(
      maximum_historic_time_between_events,
      ns - previous_event_sent.value());
    previous_event_sent = ns;

    std::string tuple_serialized = tuple.serialize_data();
    // Convert tuple_serialized to uint64_t
    uint64_t* tuple_serialized_uint64;
    memcpy(&tuple_serialized_uint64, &tuple_serialized[0], sizeof(uint64_t));

    bool has_final_states = query_evaluator->next_data(tuple_serialized_uint64);

    update_space_of_ring_tuple_queue();

    return has_final_states;
  }

  tECS::Enumerator
  create_enumerator_from_query(){
    return query_evaluator->get_enumerator_from_data();
  }

  void update_space_of_ring_tuple_queue() {
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

  RingTupleQueue::Tuple event_to_tuple(Types::Event& event) {
    if (event.event_type_id > catalog.number_of_events()) {
      throw std::runtime_error("Provided event type id is not valid.");
    }
    Types::EventInfo event_info = catalog.get_event_info(
      event.event_type_id);
    std::vector<Types::AttributeInfo> attr_infos = event_info.attributes_info;
    if (attr_infos.size() != event.attributes.size()) {
      throw std::runtime_error(
        "Event had an incorrect number of attributes");
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

  void write_int(std::shared_ptr<Types::Value>& attr) {
    Types::IntValue* val_ptr = dynamic_cast<Types::IntValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not an IntValue was provided where it "
        "should have been an IntValue!");
    int64_t* integer_ptr = queue.writer<int64_t>();
    *integer_ptr = val_ptr->val;
  }

  void write_double(std::shared_ptr<Types::Value>& attr) {
    Types::DoubleValue* val_ptr = dynamic_cast<Types::DoubleValue*>(
      attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not an DoubleValue was provided where "
        "it "
        "should have been an DoubleValue!");
    double* double_ptr = queue.writer<double>();
    *double_ptr = val_ptr->val;
  }

  void write_bool(std::shared_ptr<Types::Value>& attr) {
    Types::BoolValue* val_ptr = dynamic_cast<Types::BoolValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not an BoolValue was provided where it "
        "should have been an BoolValue!");
    bool* bool_ptr = queue.writer<bool>();
    *bool_ptr = val_ptr->val;
  }

  void write_string_view(std::shared_ptr<Types::Value>& attr) {
    Types::StringValue* val_ptr = dynamic_cast<Types::StringValue*>(
      attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not a StringValue was provided where it "
        "should have been a StringValue!");
    char* chars = queue.writer<std::string>(val_ptr->val.size());
    memcpy(chars, &val_ptr->val[0], val_ptr->val.size());
  }

  void write_date(std::shared_ptr<Types::Value>& attr) {
    Types::DateValue* val_ptr = dynamic_cast<Types::DateValue*>(attr.get());
    if (val_ptr == nullptr)
      throw std::runtime_error(
        "An attribute type that is not a DateValue was provided where it "
        "should have been a DateValue!");
    std::time_t* time_ptr = queue.writer<std::time_t>();
    *time_ptr = val_ptr->val;
  }
};
}  // namespace CORE::Internal
