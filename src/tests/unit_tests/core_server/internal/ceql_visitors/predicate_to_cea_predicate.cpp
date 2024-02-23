#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/cel_formula/predicate/and_predicate.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/inequality_predicate.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/not_predicate.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/or_predicate.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/visitors/ceql_strong_typed_predicate_to_physical_predicate.hpp"
#include "core_server/internal/ceql/value/attribute.hpp"
#include "core_server/internal/ceql/value/integer_literal.hpp"
#include "core_server/internal/ceql/value/operations/multiplication.hpp"
#include "core_server/internal/stream/ring_tuple_queue/queue.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"

using namespace RingTupleQueue;

namespace CORE::Internal::CEQL::UnitTests {

TEST_CASE("Compare with constant predicate computed correctly.", "[ValueToMathExpr]") {
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

  CEQLStrongTypedPredicateToPhysicalPredicate converter(event_info);

  SECTION("Compare with a constant") {
    std::unique_ptr<Predicate> predicate = std::make_unique<InequalityPredicate>(
      std::make_unique<Attribute>("Integer1"),
      InequalityPredicate::LogicalOperation::LESS,
      std::make_unique<IntegerLiteral>(5));
    predicate->accept_visitor(converter);
    REQUIRE((*converter.predicate)(tuple));
  }

  SECTION("Compare with an attribute") {
    std::unique_ptr<Predicate> predicate = std::make_unique<InequalityPredicate>(
      std::make_unique<Attribute>("Integer1"),
      InequalityPredicate::LogicalOperation::GREATER,
      std::make_unique<Attribute>("Integer2"));
    predicate->accept_visitor(converter);
    REQUIRE(!(*converter.predicate)(tuple));
  }

  SECTION("Compare with math_expr attribute") {
    std::unique_ptr<Predicate> predicate = std::make_unique<InequalityPredicate>(
      std::make_unique<Multiplication>(std::make_unique<Attribute>("Integer1"),
                                       std::make_unique<IntegerLiteral>(-5)),
      InequalityPredicate::LogicalOperation::GREATER,
      std::make_unique<Attribute>("Integer2"));
    predicate->accept_visitor(converter);
    REQUIRE((*converter.predicate)(tuple));
  }

  SECTION("AND predicate") {
    std::vector<std::unique_ptr<Predicate>> predicates;
    predicates.push_back(std::make_unique<InequalityPredicate>(
      std::make_unique<Multiplication>(std::make_unique<Attribute>("Integer"
                                                                   "1"),
                                       std::make_unique<IntegerLiteral>(-5)),
      InequalityPredicate::LogicalOperation::GREATER,
      std::make_unique<Attribute>("Integer2")));
    predicates.push_back(
      std::make_unique<InequalityPredicate>(std::make_unique<Attribute>("Integer1"),
                                            InequalityPredicate::LogicalOperation::GREATER,
                                            std::make_unique<Attribute>("Integer2")));
    std::unique_ptr<Predicate> predicate = std::make_unique<AndPredicate>(
      std::move(predicates));
    predicate->accept_visitor(converter);
    REQUIRE(!(*converter.predicate)(tuple));
  }

  SECTION("OR predicate") {
    std::vector<std::unique_ptr<Predicate>> predicates;
    predicates.push_back(std::make_unique<InequalityPredicate>(
      std::make_unique<Multiplication>(std::make_unique<Attribute>("Integer"
                                                                   "1"),
                                       std::make_unique<IntegerLiteral>(-5)),
      InequalityPredicate::LogicalOperation::GREATER,
      std::make_unique<Attribute>("Integer2")));
    predicates.push_back(
      std::make_unique<InequalityPredicate>(std::make_unique<Attribute>("Integer1"),
                                            InequalityPredicate::LogicalOperation::GREATER,
                                            std::make_unique<Attribute>("Integer2")));
    std::unique_ptr<Predicate> predicate = std::make_unique<OrPredicate>(
      std::move(predicates));
    predicate->accept_visitor(converter);
    REQUIRE((*converter.predicate)(tuple));
  }
  SECTION("NOT predicate") {
    std::vector<std::unique_ptr<Predicate>> predicates;
    predicates.push_back(std::make_unique<InequalityPredicate>(
      std::make_unique<Multiplication>(std::make_unique<Attribute>("Integer"
                                                                   "1"),
                                       std::make_unique<IntegerLiteral>(-5)),
      InequalityPredicate::LogicalOperation::GREATER,
      std::make_unique<Attribute>("Integer2")));
    predicates.push_back(
      std::make_unique<InequalityPredicate>(std::make_unique<Attribute>("Integer1"),
                                            InequalityPredicate::LogicalOperation::GREATER,
                                            std::make_unique<Attribute>("Integer2")));
    std::unique_ptr<Predicate> predicate = std::make_unique<NotPredicate>(
      std::make_unique<AndPredicate>(std::move(predicates)));
    predicate->accept_visitor(converter);
    REQUIRE((*converter.predicate)(tuple));
  }
}
}  // namespace CORE::Internal::CEQL::UnitTests
