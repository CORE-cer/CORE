#pragma once

#include <atomic>
#include <cassert>
#include <cstdint>
#include <tracy/Tracy.hpp>
#include <unordered_map>
#include <utility>

#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"

namespace CORE::Internal::Interface {

class GenericEvaluator {
  uint64_t current_stream_position = 0;
  Internal::Catalog& catalog;
  RingTupleQueue::Queue& queue;
  std::unordered_map<Types::EventTypeId, uint64_t> indexes{};

 protected:
  CEA::DetCEA cea;

 public:
  std::atomic<uint64_t> time_of_expiration = 0;
  CEQL::Within::TimeWindow time_window;

  GenericEvaluator(CEA::DetCEA&& cea,
                   CEQL::Within::TimeWindow time_window,
                   Internal::Catalog& catalog,
                   RingTupleQueue::Queue& queue)
      : cea(std::move(cea)), time_window(time_window), catalog(catalog), queue(queue) {}

 protected:
  uint64_t tuple_time(RingTupleQueue::Tuple& tuple) {
    ZoneScopedN("Interface::GenericEvaluator::tuple_time");
    uint64_t time;
    switch (time_window.mode) {
      case CEQL::Within::TimeWindowMode::NONE:
      case CEQL::Within::TimeWindowMode::EVENTS:
        time = current_stream_position++;
        break;
      case CEQL::Within::TimeWindowMode::NANOSECONDS:
        time = tuple.nanoseconds();
        break;
      case CEQL::Within::TimeWindowMode::ATTRIBUTE: {
        Types::EventTypeId event_type_id = tuple.id();
        auto index = indexes.find(event_type_id);
        if (index == indexes.end()) [[unlikely]] {
          indexes[event_type_id] = catalog.get_index_attribute(event_type_id,
                                                               time_window.attribute_name);
        }
        uint64_t attribute_index = indexes[event_type_id];
        time = *tuple[attribute_index];
        break;
      }
      default:
        assert(false && "Unknown time_window mode in next_data.");
        break;
    }

    return time;
  }
};
}  // namespace CORE::Internal::Interface
