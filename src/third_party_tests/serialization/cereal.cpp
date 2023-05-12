#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "shared/serializer/cereal_serializer.hpp"

namespace CerealThirdPartyTesting {
using namespace InternalCORE;

struct SimpleStruct {
  int x;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(x);
  }
};

struct VectorStruct {
  std::vector<int> values;

  VectorStruct() {}

  VectorStruct(std::initializer_list<int> vals)
    : values(vals) {}

  template <class Archive>
  void serialize(Archive& archive) {
    archive(values);
  }
};

TEST_CASE("Serialization works for a simple structure", "[cereal]") {
  SimpleStruct initial_struct{5};
  std::string serialized_struct =
    CerealSerializer<SimpleStruct>::serialize(initial_struct);
  SimpleStruct final_struct =
    CerealSerializer<SimpleStruct>::deserialize(serialized_struct);
  REQUIRE(initial_struct.x == final_struct.x);
}

TEST_CASE("Serialization works for a vector structure", "[cereal]") {
  VectorStruct initial_struct{5, 4, 5, 6};
  std::string serialized_struct =
    CerealSerializer<VectorStruct>::serialize(initial_struct);
  VectorStruct final_struct =
    CerealSerializer<VectorStruct>::deserialize(serialized_struct);
  REQUIRE(initial_struct.values == final_struct.values);
}

}  // namespace CerealThirdPartyTesting
