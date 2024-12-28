#pragma once

#include <atomic>
#include <cassert>
#include <cstdint>
#include <tracy/Tracy.hpp>
#include <unordered_map>
#include <utility>

#include "core_server/internal/ceql/query/within.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/det_cea/det_cea.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Interface::Module::Query {

class GenericEvaluator {
  uint64_t current_stream_position = 0;
  Internal::QueryCatalog& query_catalog;
  std::unordered_map<Types::UniqueEventTypeId, uint64_t> indexes{};

 protected:
  CEA::DetCEA cea;

 public:
  std::atomic<uint64_t> time_of_expiration = 0;
  CEQL::Within::TimeWindow time_window;

  GenericEvaluator(CEA::DetCEA&& cea,
                   CEQL::Within::TimeWindow time_window,
                   Internal::QueryCatalog& query_catalog)
      : cea(std::move(cea)), time_window(time_window), query_catalog(query_catalog) {}

 protected:
  uint64_t event_time(Types::EventWrapper& event) {
    ZoneScopedN("Interface::GenericEvaluator::tuple_time");
    uint64_t time;
    switch (time_window.mode) {
      case CEQL::Within::TimeWindowMode::NONE:
      case CEQL::Within::TimeWindowMode::EVENTS:
        time = current_stream_position++;
        break;
      case CEQL::Within::TimeWindowMode::NANOSECONDS:
        time = event.get_primary_time().val;
        break;
      case CEQL::Within::TimeWindowMode::ATTRIBUTE: {
        Types::UniqueEventTypeId event_type_id = event.get_unique_event_type_id();
        auto index = indexes.find(event_type_id);
        if (index == indexes.end()) [[unlikely]] {
          indexes[event_type_id] = query_catalog
                                     .get_index_attribute(event_type_id,
                                                          time_window.attribute_name);
        }
        uint64_t attribute_index = indexes[event_type_id];
        time = event.get_attribute_at_index<Types::IntValue>(attribute_index).val;
        break;
      }
      default:
        assert(false && "Unknown time_window mode in next_data.");
        break;
    }

    return time;
  }
};
}  // namespace CORE::Internal::Interface::Module::Query
