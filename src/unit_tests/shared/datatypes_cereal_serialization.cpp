#include <catch2/catch_test_macros.hpp>

#include "shared/datatypes/value/all_value_headers.hpp"
#include "shared/serializer/cereal_serializer.hpp"

namespace CerealThirdPartyTesting {
using namespace InternalCORE;
using namespace CORETypes;

template <typename Object, typename ArgumentType>
void check_serialization_of(ArgumentType arg) {
  Object initial_obj(arg);

  // Serialize
  std::string serialized_object =
    CerealSerializer<Object>::serialize(initial_obj);

  // Deserialize
  Object final_obj =
    CerealSerializer<Object>::deserialize(serialized_object);

  REQUIRE(initial_obj == final_obj);
}

template <typename Object>
void check_serialization_of() {
  Object initial_obj;

  // Serialize
  std::string serialized_object =
    CerealSerializer<Object>::serialize(initial_obj);

  // Deserialize
  Object final_obj =
    CerealSerializer<Object>::deserialize(serialized_object);

  REQUIRE(initial_obj == final_obj);
}

std::unique_ptr<Value> create_left_value() {
  return std::make_unique<Addition>(
    std::make_unique<DoubleLiteral>(1.42),
    std::make_unique<Subtraction>(
      std::make_unique<DoubleLiteral>(8.1),
      std::make_unique<DoubleLiteral>(1.65421)));
}

std::unique_ptr<Value> create_right_value() {
  return std::make_unique<StringLiteral>("Some string literal");
}

template <typename Object>
void check_serialization_of_lhs_rhs() {
  Object initial_obj(create_left_value(), create_right_value());

  // Serialize
  std::string serialized_object =
    CerealSerializer<Object>::serialize(initial_obj);

  // Deserialize
  Object final_obj =
    CerealSerializer<Object>::deserialize(serialized_object);

  INFO(initial_obj.to_string());
  INFO(final_obj.to_string());
  REQUIRE(initial_obj == final_obj);
}

template <typename Object>
void check_serialization_of_unary() {
  Object initial_obj(create_left_value());

  // Serialize
  std::string serialized_object =
    CerealSerializer<Object>::serialize(initial_obj);

  // Deserialize
  Object final_obj =
    CerealSerializer<Object>::deserialize(serialized_object);

  INFO(initial_obj.to_string());
  INFO(final_obj.to_string());
  REQUIRE(initial_obj == final_obj);
}


TEST_CASE("Serialization works for all basic CORE types",
          "[cereal, core_types]") {
  check_serialization_of<Attribute, std::string>("some attribute");
  check_serialization_of<BooleanLiteral, bool>(true);
  check_serialization_of<DoubleLiteral, double>(1.242);
  check_serialization_of<IntegerLiteral, int>(313243);
  check_serialization_of<LongLiteral, long>(100000002123);
  check_serialization_of<StringLiteral, std::string>("some string");
  check_serialization_of<Undefined>();
}

TEST_CASE("Serialization works for all composite CORE types",
          "[cereal, core_types]") {
  check_serialization_of_lhs_rhs<Addition>();
  check_serialization_of_lhs_rhs<Modulo>();
  check_serialization_of_lhs_rhs<Division>();
  check_serialization_of_lhs_rhs<Multiplication>();
  check_serialization_of_lhs_rhs<Subtraction>();

  check_serialization_of_unary<Negation>();
}
}  // namespace CerealThirdPartyTesting
