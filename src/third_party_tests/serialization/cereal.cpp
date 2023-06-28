#include <catch2/catch_test_macros.hpp>

#include "shared/serializer/cereal_serializer.hpp"
#include "third_party_tests/serialization/cereal_test_classes.hpp"

namespace CerealThirdPartyTesting {
using namespace InternalCORE;

TEST_CASE("Serialization works for a simple structure", "[cereal]") {
  SimpleStruct initial_struct{5};
  std::string serialized_struct = CerealSerializer<SimpleStruct>::serialize(
    initial_struct);
  SimpleStruct final_struct = CerealSerializer<SimpleStruct>::deserialize(
    serialized_struct);
  REQUIRE(initial_struct.x == final_struct.x);
}

TEST_CASE("Serialization works for a vector structure", "[cereal]") {
  VectorStruct initial_struct{5, 4, 5, 6};
  std::string serialized_struct = CerealSerializer<VectorStruct>::serialize(
    initial_struct);
  VectorStruct final_struct = CerealSerializer<VectorStruct>::deserialize(
    serialized_struct);
  REQUIRE(initial_struct.values == final_struct.values);
}

TEST_CASE("Serialization works for an object with an abstract class ptr",
          "[cereal]") {
  std::shared_ptr<ConcreteClass1> obj1(new ConcreteClass1{5, 4, 3, 2});
  std::shared_ptr<ConcreteClass2> obj2(new ConcreteClass2(10));
  ClassWithAbstractClassSharedPtr initial_object{{obj1, obj2}};

  // Serialize
  std::string serialized_object = CerealSerializer<
    ClassWithAbstractClassSharedPtr>::serialize(initial_object);

  // Deserialize
  ClassWithAbstractClassSharedPtr final_object = CerealSerializer<
    ClassWithAbstractClassSharedPtr>::deserialize(serialized_object);

  REQUIRE(initial_object.objects.size() == final_object.objects.size());
  // Detailed checks

  auto final_concrete1 = std::dynamic_pointer_cast<ConcreteClass1>(
    final_object.objects[0]);
  REQUIRE(final_concrete1);
  REQUIRE(obj1->values == final_concrete1->values);

  auto final_concrete2 = std::dynamic_pointer_cast<ConcreteClass2>(
    final_object.objects[1]);
  REQUIRE(final_concrete2);
  REQUIRE(obj2->value == final_concrete2->value);
}

}  // namespace CerealThirdPartyTesting
