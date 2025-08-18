#include "common.hpp"

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstdint>
#include <string>

#include "core_server/internal/interface/backend.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
bool is_the_same_as(Types::Event event, uint64_t event_type_id, std::string name) {
  if (event.get_event_type_id() != event_type_id) {
    return false;
  }
  return event.attributes[0]->to_string() == name;
}

bool is_the_same_as(Types::Event event,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value) {
  if (event.get_event_type_id() != event_type_id) {
    return false;
  }
  return (event.attributes[0]->to_string() == name
          && std::stoll(event.attributes[1]->to_string()) == value);
}

bool is_the_same_as(Types::Event event,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value1,
                    int64_t value2) {
  if (event.get_event_type_id() != event_type_id) {
    return false;
  }
  return (event.attributes[0]->to_string() == name
          && std::stoll(event.attributes[1]->to_string()) == value1
          && std::stoll(event.attributes[2]->to_string()) == value2);
}

Types::StreamInfo basic_stock_declaration(Interface::Backend<>& backend) {
  Types::StreamInfo stream_info = backend.add_stream_type(
    {"Stock",
     {{"SELL",
       {{"name", Types::ValueTypes::STRING_VIEW}, {"price", Types::ValueTypes::INT64}}},
      {"BUY",
       {{"name", Types::ValueTypes::STRING_VIEW}, {"price", Types::ValueTypes::INT64}}}}});

  return stream_info;
}

Types::StreamInfo primary_time_stock_declaration(Interface::Backend<>& backend) {
  Types::StreamInfo stream_info = backend.add_stream_type(
    {"Stock",
     {{"SELL",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"time", Types::ValueTypes::PRIMARY_TIME}}},
      {"BUY",
       {{"name", Types::ValueTypes::STRING_VIEW},
        {"price", Types::ValueTypes::INT64},
        {"time", Types::ValueTypes::PRIMARY_TIME}}}}});

  return stream_info;
}
}  // namespace CORE::Internal::Evaluation::UnitTests
