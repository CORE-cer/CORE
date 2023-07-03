#include <catch2/catch_test_macros.hpp>

#include "shared/datatypes/value.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CORE {
namespace Internal {
namespace ThirdPartyTests {

template <typename Object, typename ArgumentType>
void check_serialization_of(ArgumentType arg) {
  Object initial_obj(arg);

  // Serialize
  std::string serialized_object = CerealSerializer<Object>::serialize(
    initial_obj);

  // Deserialize
  Object final_obj = CerealSerializer<Object>::deserialize(
    serialized_object);

  REQUIRE(initial_obj.val == final_obj.val);
}

TEST_CASE("Serialization works for all basic CORE types",
          "[cereal, core_types]") {
  check_serialization_of<Types::StringValue, std::string>("some attribute");
  check_serialization_of<Types::BoolValue, bool>(true);
  check_serialization_of<Types::DoubleValue, double>(1.242);
  check_serialization_of<Types::IntValue, int64_t>(313243);
  check_serialization_of<Types::DateValue, std::time_t>(200);
}

}  // namespace ThirdPartyTests
}  // namespace Internal
}  // namespace CORE
