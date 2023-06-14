#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/cea/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/cea/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/cea/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"

using namespace RingTupleQueue;
using namespace InternalCORECEQL;
using namespace InternalCORECEA;

namespace COREQueryParsingTestsValueToMathExpr {

TEST_CASE("CEQL physical_predicate to CEA physical_predicate", "[ValueToMathExpr]") {
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

  SECTION("Compare with constant", "ValueToMathExpr") {
    // clang-format off
    CompareWithConstant<ComparisonType::EQUALS, int64_t>
    predicate_equals(1, -1);
    REQUIRE(predicate_equals(tuple));

    CompareWithConstant<ComparisonType::GREATER, int64_t>
    predicate_greater(1, -2);
    REQUIRE(predicate_greater(tuple));

    CompareWithConstant<ComparisonType::GREATER_EQUALS, int64_t>
    predicate_greater_equals(1, -1);
    REQUIRE(predicate_greater_equals(tuple));

    CompareWithConstant<ComparisonType::LESS_EQUALS, int64_t>
    predicate_less_equals(1, -1);
    REQUIRE(predicate_less_equals(tuple));

    CompareWithConstant<ComparisonType::LESS, int64_t>
    predicate_less(1, 1);
    REQUIRE(predicate_less(tuple));

    CompareWithConstant<ComparisonType::NOT_EQUALS, int64_t>
    predicate_not_equals(1, -3);
    REQUIRE(predicate_not_equals(tuple));
    // clang-format on
  }

  SECTION("Compare with attribute", "ValueToMathExpr") {
    // clang-format off
    CompareWithAttribute<ComparisonType::EQUALS, int64_t>
    predicate_equals(1, 2);
    REQUIRE(!predicate_equals(tuple));

    CompareWithAttribute<ComparisonType::GREATER, int64_t>
    predicate_greater(2, 1);
    REQUIRE(predicate_greater(tuple));

    CompareWithAttribute<ComparisonType::GREATER_EQUALS, int64_t>
    predicate_greater_equals(1, 2);
    REQUIRE(!predicate_greater_equals(tuple));

    CompareWithAttribute<ComparisonType::LESS_EQUALS, int64_t>
    predicate_less_equals(1, 2);
    REQUIRE(predicate_less_equals(tuple));

    CompareWithAttribute<ComparisonType::LESS, int64_t>
    predicate_less(1, 2);
    REQUIRE(predicate_less(tuple));

    CompareWithAttribute<ComparisonType::NOT_EQUALS, int64_t>
    predicate_not_equals(1, 2);
    REQUIRE(predicate_not_equals(tuple));
    // clang-format on
  }

  SECTION("Compare math_exprs", "ValueToMathExpr") {
    // clang-format off
    auto math_expr_eval_1 =
      std::make_unique<InternalCORECEA::Subtraction<int64_t>>(
          std::make_unique<InternalCORECEA::Attribute<int64_t>>(1),
          std::make_unique<InternalCORECEA::Literal<int64_t>>(-2));
    auto math_expr_eval_2 =
      std::make_unique<InternalCORECEA::Multiplication<int64_t>>(
          std::make_unique<InternalCORECEA::Attribute<int64_t>>(1),
          std::make_unique<InternalCORECEA::Literal<int64_t>>(-2));
    auto math_expr_eval_4 =
      std::make_unique<InternalCORECEA::Multiplication<int64_t>>(
          math_expr_eval_2->clone(),
          math_expr_eval_2->clone());

    auto second_math_expr_eval_2 =
      std::make_unique<InternalCORECEA::Division<int64_t>>(
          math_expr_eval_4->clone(),
          math_expr_eval_2->clone());

    CompareMathExprs<ComparisonType::EQUALS, int64_t>
    predicate_equals(second_math_expr_eval_2->clone(),
                     math_expr_eval_2->clone());
    REQUIRE(predicate_equals(tuple));

    CompareMathExprs<ComparisonType::GREATER, int64_t>
    predicate_greater(math_expr_eval_4->clone(), math_expr_eval_1->clone());
    REQUIRE(predicate_greater(tuple));

    CompareMathExprs<ComparisonType::GREATER_EQUALS, int64_t>
    predicate_greater_equals(second_math_expr_eval_2->clone(),
                             math_expr_eval_2->clone());
    REQUIRE(predicate_greater_equals(tuple));

    CompareMathExprs<ComparisonType::LESS_EQUALS, int64_t>
    predicate_less_equals(second_math_expr_eval_2->clone(),
                          math_expr_eval_2->clone());
    REQUIRE(predicate_less_equals(tuple));

    CompareMathExprs<ComparisonType::LESS, int64_t>
    predicate_less(math_expr_eval_1->clone(),
                   math_expr_eval_2->clone());
    REQUIRE(predicate_less(tuple));

    CompareMathExprs<ComparisonType::NOT_EQUALS, int64_t>
    predicate_not_equals(math_expr_eval_1->clone(),
                         math_expr_eval_2->clone());
    REQUIRE(predicate_not_equals(tuple));
    // clang-format on
  }
}
}  // namespace COREQueryParsingTestsValueToMathExpr
