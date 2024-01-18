#include "common.hpp"

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <condition_variable>

namespace CORE::Internal::Evaluation::UnitTests {
bool is_the_same_as(Types::Event event,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value) {
  if (event.event_type_id != event_type_id) {
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
  if (event.event_type_id != event_type_id) {
    return false;
  }
  return (event.attributes[0]->to_string() == name
          && std::stoll(event.attributes[1]->to_string()) == value1
          && std::stoll(event.attributes[2]->to_string()) == value2);
}
}  // namespace CORE::Internal::Evaluation::UnitTests
