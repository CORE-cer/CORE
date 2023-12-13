#pragma once

#include <functional>
#include <string>
#include <vector>

#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/datatypes/aliases/port_number.hpp"

namespace CORE::External {
class BackendInterface {
 public:
  virtual ~BackendInterface(){};
  virtual Types::EventTypeId
  declare_event(std::string event_name,
                std::vector<Types::AttributeInfo> attributes_info) = 0;

  virtual Types::EventInfo
  get_event_info(Types::EventTypeId event_type_id) = 0;

  virtual Types::EventInfo get_event_info(std::string event_name) = 0;

  virtual std::vector<Types::EventInfo> get_all_events_info() = 0;

  virtual Types::StreamTypeId
  declare_stream(std::string stream_name,
                 std::vector<Types::EventTypeId> event_types) = 0;

  virtual Types::StreamInfo
  get_stream_info(Types::StreamTypeId event_type_id) = 0;

  virtual Types::StreamInfo get_stream_info(std::string event_name) = 0;

  virtual std::vector<Types::StreamInfo> get_all_streams_info() = 0;

  virtual void declare_query(std::string query,
                             std::function<void(Types::Enumerator)> result_handler) = 0;

  virtual void send_event_to_queries(Types::StreamTypeId stream_id,
                                     Types::Event event) = 0;
};
}  // namespace CORE::External
