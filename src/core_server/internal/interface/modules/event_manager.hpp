#pragma once

#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"
#include "tracy/Tracy.hpp"

namespace CORE::Internal::Interface::Module {

class EventManager {
  Catalog& catalog;
  RingTupleQueue::Queue& queue;

 public:
  EventManager(Catalog& catalog, RingTupleQueue::Queue& queue)
      : catalog(catalog), queue(queue) {}

  RingTupleQueue::Tuple event_to_tuple(const Types::Event& event) {
    ZoneScopedN("Backend::event_to_tuple");
    if (event.event_type_id > catalog.number_of_events()) {
      throw std::runtime_error("Provided event type id is not valid.");
    }
    Types::EventInfo event_info = catalog.get_event_info(event.event_type_id);
    std::vector<Types::AttributeInfo> attr_infos = event_info.attributes_info;
    if (attr_infos.size() != event.attributes.size()) {
      throw std::runtime_error("Event had an incorrect number of attributes");
    }

    uint64_t* data;

    if (event.primary_time.has_value()) {
      int64_t primary_time = event.primary_time.value()->val;
      std::chrono::system_clock::time_point primary_time_tp{
        std::chrono::nanoseconds(primary_time)};
      data = queue.start_tuple(event.event_type_id, primary_time_tp);
    } else {
      data = queue.start_tuple(event.event_type_id);
    }

    for (size_t i = 0; i < attr_infos.size(); i++) {
      auto& attr_info = attr_infos[i];
      // TODO: Why is this a shared_ptr?
      std::shared_ptr<Types::Value> attr = event.attributes[i];
      switch (attr_info.value_type) {
        case Types::INT64:
        case Types::PRIMARY_TIME:
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
          assert(
            false
            && "A value type was added but not updated in the switch in event_to_tuple");
      }
    }
    return queue.get_tuple(data);
  }

 private:
  // void update_space_of_ring_tuple_queue() {
  //   if (query_events_expiration_time.size() != 0) {
  //     assert(query_events_expiration_time.size() == query_events_time_window_mode.size());
  //     uint64_t consensus = UINT64_MAX;
  //     for (size_t i = 0; i < query_events_time_window_mode.size(); i++) {
  //       switch (query_events_time_window_mode[i]) {
  //         case CEQL::Within::TimeWindowMode::EVENTS:
  //           consensus = std::min(query_events_expiration_time[i].get()
  //                                  * maximum_historic_time_between_events,
  //                                consensus);
  //         case CEQL::Within::TimeWindowMode::ATTRIBUTE:
  //           consensus = 0;
  //           break;
  //         case CEQL::Within::TimeWindowMode::NONE:
  //         case CEQL::Within::TimeWindowMode::NANOSECONDS:
  //           consensus = std::min(query_events_expiration_time[i].get().load(), consensus);
  //           break;
  //         default:
  //           assert(false
  //                  && "Unknown time_window mode in update_space_of_ring_tuple_queue.");
  //           break;
  //       }
  //     }
  //     queue.update_overwrite_timepoint(consensus);
  //   }
  // }

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
    Types::DoubleValue* val_ptr = dynamic_cast<Types::DoubleValue*>(attr.get());
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
    Types::StringValue* val_ptr = dynamic_cast<Types::StringValue*>(attr.get());
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
}  // namespace CORE::Internal::Interface::Module
