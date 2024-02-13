#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "core_server/internal/ceql/value/all_value_headers.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"

namespace CORE::Internal::CEQL::UnitTests {

TEST_CASE("CEQL physical_predicate to CEA physical_predicate", "[ValueToMathExpr]") {
  std::vector<Types::AttributeInfo> attributes_info;
  attributes_info.emplace_back("String", Types::ValueTypes::STRING_VIEW);
  attributes_info.emplace_back("Integer1", Types::ValueTypes::INT64);
  attributes_info.emplace_back("Integer2", Types::ValueTypes::INT64);
  attributes_info.emplace_back("Double1", Types::ValueTypes::DOUBLE);
  attributes_info.emplace_back("Double2", Types::ValueTypes::DOUBLE);
  Types::CatalogEventInfo event_info(0, "some_event_name", std::move(attributes_info));

  RingTupleQueue::TupleSchemas schemas;

  RingTupleQueue::Queue ring_tuple_queue(100, &schemas);
  auto id = schemas.add_schema({RingTupleQueue::SupportedTypes::STRING_VIEW,
                                RingTupleQueue::SupportedTypes::INT64,
                                RingTupleQueue::SupportedTypes::INT64,
                                RingTupleQueue::SupportedTypes::DOUBLE,
                                RingTupleQueue::SupportedTypes::DOUBLE});
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

  RingTupleQueue::Tuple tuple(data, &schemas);

  SECTION("Compare with constant", "ValueToMathExpr") {
    // clang-format off
    CEA::CompareWithConstant<CEA::ComparisonType::EQUALS, int64_t>
    predicate_equals(id, 1, -1);
    REQUIRE(predicate_equals(tuple));

    CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>
    predicate_greater(id, 1, -2);
    REQUIRE(predicate_greater(tuple));

    CEA::CompareWithConstant<CEA::ComparisonType::GREATER_EQUALS, int64_t>
    predicate_greater_equals(id, 1, -1);
    REQUIRE(predicate_greater_equals(tuple));

    CEA::CompareWithConstant<CEA::ComparisonType::LESS_EQUALS, int64_t>
    predicate_less_equals(id, 1, -1);
    REQUIRE(predicate_less_equals(tuple));

    CEA::CompareWithConstant<CEA::ComparisonType::LESS, int64_t>
    predicate_less(id, 1, 1);
    REQUIRE(predicate_less(tuple));

    CEA::CompareWithConstant<CEA::ComparisonType::NOT_EQUALS, int64_t>
    predicate_not_equals(id, 1, -3);
    REQUIRE(predicate_not_equals(tuple));
    // clang-format on
  }

  SECTION("Compare with attribute", "ValueToMathExpr") {
    // clang-format off
    CEA::CompareWithAttribute<CEA::ComparisonType::EQUALS, int64_t, int64_t>
    predicate_equals(id, 1, 2);
    REQUIRE(!predicate_equals(tuple));

    CEA::CompareWithAttribute<CEA::ComparisonType::GREATER, int64_t, int64_t>
    predicate_greater(id, 2, 1);
    REQUIRE(predicate_greater(tuple));

    CEA::CompareWithAttribute<CEA::ComparisonType::GREATER_EQUALS, int64_t, int64_t>
    predicate_greater_equals(id, 1, 2);
    REQUIRE(!predicate_greater_equals(tuple));

    CEA::CompareWithAttribute<CEA::ComparisonType::LESS_EQUALS, int64_t, int64_t>
    predicate_less_equals(id, 1, 2);
    REQUIRE(predicate_less_equals(tuple));

    CEA::CompareWithAttribute<CEA::ComparisonType::LESS, int64_t, int64_t>
    predicate_less(id, 1, 2);
    REQUIRE(predicate_less(tuple));

    CEA::CompareWithAttribute<CEA::ComparisonType::NOT_EQUALS, int64_t, int64_t>
    predicate_not_equals(id, 1, 2);
    REQUIRE(predicate_not_equals(tuple));
    // clang-format on
  }

  SECTION("Compare math_exprs", "ValueToMathExpr") {
    // clang-format off
    auto math_expr_eval_1 =
      std::make_unique<CEA::Subtraction<int64_t>>(
          std::make_unique<CEA::Attribute<int64_t, int64_t>>(1),
          std::make_unique<CEA::Literal<int64_t>>(-2));
    auto math_expr_eval_2 =
      std::make_unique<CEA::Multiplication<int64_t>>(
          std::make_unique<CEA::Attribute<int64_t, int64_t>>(1),
          std::make_unique<CEA::Literal<int64_t>>(-2));
    auto math_expr_eval_4 =
      std::make_unique<CEA::Multiplication<int64_t>>(
          math_expr_eval_2->clone(),
          math_expr_eval_2->clone());

    auto second_math_expr_eval_2 =
      std::make_unique<CEA::Division<int64_t>>(
          math_expr_eval_4->clone(),
          math_expr_eval_2->clone());

    CEA::CompareMathExprs<CEA::ComparisonType::EQUALS, int64_t>
    predicate_equals(id, second_math_expr_eval_2->clone(),
                     math_expr_eval_2->clone());
    REQUIRE(predicate_equals(tuple));

    CEA::CompareMathExprs<CEA::ComparisonType::GREATER, int64_t>
    predicate_greater(id, math_expr_eval_4->clone(), math_expr_eval_1->clone());
    REQUIRE(predicate_greater(tuple));

    CEA::CompareMathExprs<CEA::ComparisonType::GREATER_EQUALS, int64_t>
    predicate_greater_equals(id, second_math_expr_eval_2->clone(),
                             math_expr_eval_2->clone());
    REQUIRE(predicate_greater_equals(tuple));

    CEA::CompareMathExprs<CEA::ComparisonType::LESS_EQUALS, int64_t>
    predicate_less_equals(id, second_math_expr_eval_2->clone(),
                          math_expr_eval_2->clone());
    REQUIRE(predicate_less_equals(tuple));

    CEA::CompareMathExprs<CEA::ComparisonType::LESS, int64_t>
    predicate_less(id, math_expr_eval_1->clone(),
                   math_expr_eval_2->clone());
    REQUIRE(predicate_less(tuple));

    CEA::CompareMathExprs<CEA::ComparisonType::NOT_EQUALS, int64_t>
    predicate_not_equals(id, math_expr_eval_1->clone(),
                         math_expr_eval_2->clone());
    REQUIRE(predicate_not_equals(tuple));
    // clang-format on
  }
}
}  // namespace CORE::Internal::CEQL::UnitTests
