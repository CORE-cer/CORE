#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/tecs.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"
#include "unit_tests/core_server/internal/evaluation/evaluation_algorithm/common.hpp"

namespace CORE::Internal::Evaluation::UnitTests {
TEST_CASE("Evaluation on the example stream of the paper.") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      20,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};

  REQUIRE(outputs.size() == 4);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1920));

  REQUIRE(outputs[1].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1920));

  REQUIRE(outputs[2].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[2].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[2].second[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(outputs[2].second[2], 0, "AMZN", 1920));

  REQUIRE(outputs[3].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[3].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[3].second[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(outputs[3].second[2], 0, "AMZN", 1920));
  next_output_enumerator = {};  // To prevent segfault
}

TEST_CASE(
  "Evaluation on the example stream of the paper with within of 4 "
  "seconds.") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft; SELL as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea = CEA::DetCEA(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      5,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);

  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};

  REQUIRE(outputs.size() == 2);

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 6);

  REQUIRE(outputs[1].first.first == 1);
  REQUIRE(outputs[1].first.second == 6);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1920));

  REQUIRE(outputs[1].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1920));
  next_output_enumerator = {};  // To prevent segfault
}

TEST_CASE("Evaluation of a query with contiguous events") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());

  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));

  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t expiration_time = 0;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      20,
                      expiration_time);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 7);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 7);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 8);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 9);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 0);
}

TEST_CASE("Evaluation of long query") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL as msft: SELL as intel: SELL as amzn: SELL as msft: SELL "
    "as intel: SELL as amzn: SELL as msft: SELL as intel: SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND intel[name='INTL'] AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 7);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 8);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 9);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 10);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 11);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 12);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 13);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 13);

  REQUIRE(outputs[0].second.size() == 9);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[3], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[4], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[5], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[6], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[7], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[8], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 14);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 15);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 0);
}

TEST_CASE("Evaluation of long query with continuous and OR") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE SELL: (SELL OR BUY): (SELL OR BUY): (SELL OR BUY): (SELL OR "
    "BUY): (SELL OR BUY): (SELL OR BUY): (SELL OR BUY)";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 7);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 8);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 9);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 10);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 11);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 12);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 0);
  REQUIRE(outputs[0].first.second == 12);

  REQUIRE(outputs[0].second.size() == 8);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 1, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[3], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[4], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[5], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[6], 1, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[7], 1, "INTL", 80));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 14);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 15);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 0);
}

TEST_CASE("Evaluation of longer query with continuous and OR v2") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL OR BUY): (SELL OR BUY): (SELL OR BUY): (SELL OR BUY): "
    "(SELL OR BUY): (SELL OR BUY): (SELL OR BUY): (SELL OR BUY): (SELL OR "
    "BUY): (SELL OR BUY): (SELL OR BUY): (SELL OR BUY): (SELL OR BUY): "
    "(SELL OR BUY): (SELL OR BUY): (SELL OR BUY)";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 7);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 8);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 9);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 10);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 11);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 12);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 13);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 14);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 15);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 0);
  REQUIRE(outputs[0].first.second == 15);

  REQUIRE(outputs[0].second.size() == 16);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 1, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[3], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[4], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[5], 0, "AMZN", 1900));
  REQUIRE(is_the_same_as(outputs[0].second[6], 1, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[7], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[8], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[9], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[10], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[11], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[12], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[13], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[14], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[15], 1, "INTL", 80));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 16);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 17);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};  // To prevent segfault
  REQUIRE(outputs.size() == 1);
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous sequencing, contiguous "
  "iteration, and "
  "OR") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL):+ as msft; (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 6);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);
  REQUIRE(outputs[2].first.first == 0);
  REQUIRE(outputs[2].first.second == 4);
  REQUIRE(outputs[3].first.first == 1);
  REQUIRE(outputs[3].first.second == 4);
  REQUIRE(outputs[4].first.first == 0);
  REQUIRE(outputs[4].first.second == 4);
  REQUIRE(outputs[5].first.first == 0);
  REQUIRE(outputs[5].first.second == 4);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[3], 0, "AMZN", 1900));

  REQUIRE(outputs[2].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[2].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[2].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[2].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[3].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[3].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[3].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[3].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[4].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[4].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[4].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[4].second[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[4].second[3], 0, "AMZN", 1900));

  REQUIRE(outputs[5].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[5].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[5].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[5].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 105);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 85);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1901);
  next_output_enumerator = evaluator.next(tuple, 7);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};

  // NOTE: If fails, should check if correct. Made assuming correctness

  REQUIRE(outputs.size() == 10);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 7);
  REQUIRE(outputs[1].first.first == 1);
  REQUIRE(outputs[1].first.second == 7);
  REQUIRE(outputs[2].first.first == 0);
  REQUIRE(outputs[2].first.second == 7);
  REQUIRE(outputs[3].first.first == 0);
  REQUIRE(outputs[3].first.second == 7);
  REQUIRE(outputs[4].first.first == 1);
  REQUIRE(outputs[4].first.second == 7);
  REQUIRE(outputs[5].first.first == 0);
  REQUIRE(outputs[5].first.second == 7);
  REQUIRE(outputs[6].first.first == 1);
  REQUIRE(outputs[6].first.second == 7);
  REQUIRE(outputs[7].first.first == 0);
  REQUIRE(outputs[7].first.second == 7);
  REQUIRE(outputs[8].first.first == 0);
  REQUIRE(outputs[8].first.second == 7);
  REQUIRE(outputs[9].first.first == 0);
  REQUIRE(outputs[9].first.second == 7);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 105));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 85));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1901));

  REQUIRE(outputs[1].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 85));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1901));

  REQUIRE(outputs[2].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[2].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[2].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[2].second[2], 0, "INTL", 85));
  REQUIRE(is_the_same_as(outputs[2].second[3], 0, "AMZN", 1901));

  REQUIRE(outputs[3].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[3].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[3].second[1], 0, "INTL", 85));
  REQUIRE(is_the_same_as(outputs[3].second[2], 0, "AMZN", 1901));

  REQUIRE(outputs[4].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[4].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[4].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[4].second[2], 0, "AMZN", 1901));

  REQUIRE(outputs[5].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[5].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[5].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[5].second[2], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[5].second[3], 0, "AMZN", 1901));

  REQUIRE(outputs[6].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[6].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[6].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[6].second[2], 0, "AMZN", 1901));

  REQUIRE(outputs[7].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[7].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[7].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[7].second[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[7].second[3], 0, "AMZN", 1901));

  REQUIRE(outputs[8].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[8].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[8].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[8].second[2], 0, "AMZN", 1901));

  REQUIRE(outputs[9].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[9].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[9].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[9].second[2], 0, "AMZN", 1901));
}

TEST_CASE(
  "Evaluation of a query with mix of contiguous iteration, contiguous "
  "sequencing, non contiguous sequencing, and "
  "OR v2") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL):+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT']\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[3], 0, "AMZN", 1900));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, contiguous "
  "sequencing, and non contiguous sequencing") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL)+: BUY: SELL";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 2);
  REQUIRE(outputs[0].first.second == 5);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 5);

  REQUIRE(outputs[0].second.size() == 3);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].second.size() == 4);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[2], 1, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[3], 0, "AMZN", 1900));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, OR, and "
  "AS") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL OR BUY)+ as msft\n"
    "FILTER msft[name='MSFT']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 2);
  REQUIRE(outputs[0].first.second == 2);

  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 2);

  REQUIRE(outputs[0].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[0].second[0], 1, "MSFT", 102));

  REQUIRE(outputs[0].second.size() == 2);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 1, "MSFT", 102));
}

TEST_CASE(
  "Evaluation of a query with mix of non contiguous iteration, and AS") {
  Catalog catalog;
  auto event_type_id_1 = catalog.add_event_type(
    "SELL",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});
  auto event_type_id_2 = catalog.add_event_type(
    "BUY",
    {{"name", Types::ValueTypes::STRING_VIEW},
     {"price", Types::ValueTypes::INT64}});

  auto stream_type = catalog.add_stream_type("Stock",
                                             {event_type_id_1,
                                              event_type_id_2});

  RingTupleQueue::Queue ring_tuple_queue(100, &catalog.tuple_schemas);

  std::string string_query =
    "SELECT * FROM Stock\n"
    "WHERE (SELL)+ as msft\n"
    "FILTER msft[name='MSFT']";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  CEQL::FormulaToLogicalCEA visitor = query_to_logical_cea(catalog, query);

  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  uint64_t event_time_of_expiration;
  Evaluator evaluator(std::move(cea),
                      std::move(tuple_evaluator),
                      100,
                      event_time_of_expiration);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 1);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  next_output_enumerator = {};
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }

  REQUIRE(outputs[0].first.first == 2);
  REQUIRE(outputs[0].first.second == 2);

  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 2);

  REQUIRE(outputs[0].second.size() == 1);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));

  REQUIRE(outputs[0].second.size() == 2);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "MSFT", 102));
}
}  // namespace CORE::Internal::Evaluation::UnitTests