#include "core_server/internal/ceql/value/visitors/value_to_math_expr.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"

using namespace RingTupleQueue;
using namespace InternalCORECEQL;
using namespace InternalCORECEA;

namespace COREQueryParsingTestsValueToMathExpr {

TEST_CASE("Single functionality testing of Value to MathExpr",
          "[ValueToMathExpr]") {
  std::vector<AttributeInfo> attributes_info;
  attributes_info.emplace_back("String",
                               CORETypes::ValueTypes::STRING_VIEW);
  attributes_info.emplace_back("Integer1", CORETypes::ValueTypes::INT64);
  attributes_info.emplace_back("Integer2", CORETypes::ValueTypes::INT64);
  attributes_info.emplace_back("Double1", CORETypes::ValueTypes::DOUBLE);
  attributes_info.emplace_back("Double2", CORETypes::ValueTypes::DOUBLE);
  EventInfo event_info(0, "some_event_name", std::move(attributes_info));

  TupleSchemas schemas;
  Queue ring_tuple_queue(100, &schemas);
  auto id =
      schemas.add_schema({SupportedTypes::STRING_VIEW,
                          SupportedTypes::INT64, SupportedTypes::INT64,
                          SupportedTypes::DOUBLE, SupportedTypes::DOUBLE});
  std::span<uint64_t> data = ring_tuple_queue.start_tuple(id);
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

  InternalCORECEQL::ValueToMathExpr<int64_t> value_to_int_math_expr(
      event_info);
  InternalCORECEQL::ValueToMathExpr<double> value_to_double_math_expr(
      event_info);

  SECTION("Int attributes", "ValueToMathExpr") {
    InternalCORECEQL::Attribute value("Integer1");
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == -1);
    InternalCORECEQL::Attribute value2("Integer2");
    value2.accept_visitor(value_to_int_math_expr);
    math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 1);
  }

  SECTION("double attributes", "ValueToMathExpr") {
    InternalCORECEQL::Attribute value("Double1");
    value.accept_visitor(value_to_double_math_expr);
    auto math_expr = std::move(value_to_double_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == -1.0);
    InternalCORECEQL::Attribute value2("Double2");
    value2.accept_visitor(value_to_double_math_expr);
    math_expr = std::move(value_to_double_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 1.0);
  }

  SECTION("literal", "ValueToMathExpr") {
    InternalCORECEQL::IntegerLiteral value(2);
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 2);
  }

  SECTION("addition", "ValueToMathExpr") {
    InternalCORECEQL::Addition value(
        std::make_unique<InternalCORECEQL::IntegerLiteral>(2),
        std::make_unique<InternalCORECEQL::Attribute>("Integer1"));
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    REQUIRE(math_expr->eval(tuple) == 1);
  }

  SECTION("Combination of exprs", "ValueToMathExpr") {
    InternalCORECEQL::Addition value(
        std::make_unique<InternalCORECEQL::IntegerLiteral>(2),
        std::make_unique<InternalCORECEQL::Multiplication>(
            std::make_unique<InternalCORECEQL::Subtraction>(
                std::make_unique<InternalCORECEQL::Division>(
                    std::make_unique<InternalCORECEQL::IntegerLiteral>(10),
                    std::make_unique<InternalCORECEQL::IntegerLiteral>(3)),
                std::make_unique<InternalCORECEQL::Attribute>("Integer1")),
            std::make_unique<InternalCORECEQL::Modulo>(
                std::make_unique<InternalCORECEQL::Attribute>("Integer2"),
                std::make_unique<InternalCORECEQL::IntegerLiteral>(7))));
    // 2 + ((10/3 - -1)) * (1 % 7))
    value.accept_visitor(value_to_int_math_expr);
    auto math_expr = std::move(value_to_int_math_expr.math_expr);
    INFO("Math_expr: " + math_expr->to_string());
    REQUIRE(math_expr->eval(tuple) == 6);
  }
}
}  // namespace COREQueryParsingTestsValueToMathExpr
