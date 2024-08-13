#include "core_server/internal/ceql/value/visitors/value_to_math_expr.hpp"

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/integer_literal.hpp"
#include "core_server/internal/ceql/value/operations/addition.hpp"
#include "core_server/internal/ceql/value/operations/division.hpp"
#include "core_server/internal/ceql/value/operations/modulo.hpp"
#include "core_server/internal/ceql/value/operations/multiplication.hpp"
#include "core_server/internal/ceql/value/operations/subtraction.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

using namespace RingTupleQueue;

namespace CORE::Internal::CEQL::UnitTests {

TEST_CASE("Single functionality testing of Value to MathExpr", "[ValueToMathExpr]") {
  std::vector<Types::AttributeInfo> attributes_info;
  attributes_info.emplace_back("String", Types::ValueTypes::STRING_VIEW);
  attributes_info.emplace_back("Integer1", Types::ValueTypes::INT64);
  attributes_info.emplace_back("Integer2", Types::ValueTypes::INT64);
  attributes_info.emplace_back("Double1", Types::ValueTypes::DOUBLE);
  attributes_info.emplace_back("Double2", Types::ValueTypes::DOUBLE);
  Types::EventInfo event_info(0, "some_event_name", std::move(attributes_info));

  TupleSchemas schemas;
  Queue ring_tuple_queue(100, &schemas);
  auto id = schemas.add_schema({SupportedTypes::STRING_VIEW,
                                SupportedTypes::INT64,
                                SupportedTypes::INT64,
                                SupportedTypes::DOUBLE,
                                SupportedTypes::DOUBLE});
  uint64_t* data = ring_tuple_queue.start_tuple(id);
  char* chars = ring_tuple_queue.writer<std::string>(11);
  std::string hello_world = "hello world";
  memcpy(chars, &hello_world[0], 11);
  int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = -1;
  integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = 1;
  double* double_ptr = ring_tuple_queue.writer<double>();
  *double_ptr = -1.0;
  double_ptr = ring_tuple_queue.writer<double>();
  *double_ptr = 1.0;

  Tuple tuple(data, &schemas);

  ValueToMathExpr<int64_t> value_to_int_math_expr(event_info);
  ValueToMathExpr<double> value_to_double_math_expr(event_info);

  SECTION("Int attributes", "ValueToMathExpr") {
    Attribute value("Integer1");
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == -1); // NOLINT
    Attribute value2("Integer2");
    value2.accept_visitor(value_to_int_math_expr);
    math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 1); // NOLINT
  }

  SECTION("double attributes", "ValueToMathExpr") {
    Attribute value("Double1");
    value.accept_visitor(value_to_double_math_expr);
    auto math_expr = std::move(value_to_double_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == -1.0); // NOLINT
    Attribute value2("Double2");
    value2.accept_visitor(value_to_double_math_expr);
    math_expr = std::move(value_to_double_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 1.0); // NOLINT
  }

  SECTION("literal", "ValueToMathExpr") {
    IntegerLiteral value(2);
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 2); // NOLINT
  }

  SECTION("addition", "ValueToMathExpr") {
    Addition value(std::make_unique<IntegerLiteral>(2),
                   std::make_unique<Attribute>("Integer1"));
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 1); // NOLINT
  }

  SECTION("Combination of exprs", "ValueToMathExpr") {
    Addition value(std::make_unique<IntegerLiteral>(2),
                   std::make_unique<Multiplication>(
                     std::make_unique<Subtraction>(
                       std::make_unique<Division>(std::make_unique<IntegerLiteral>(10),
                                                  std::make_unique<IntegerLiteral>(3)),
                       std::make_unique<Attribute>("Integer1")),
                     std::make_unique<Modulo>(std::make_unique<Attribute>("Integer2"),
                                              std::make_unique<IntegerLiteral>(7))));
    // 2 + ((10/3 - -1)) * (1 % 7))
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    INFO("Math_expr: " + math_expr->to_string());
    REQUIRE(math_expr->eval(tuple) == 6); // NOLINT
  }
}
}  // namespace CORE::Internal::CEQL::UnitTests
