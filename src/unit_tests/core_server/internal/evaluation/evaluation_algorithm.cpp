#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>

#include "core_server/internal/ceql/cel_formula/formula/visitors/formula_to_logical_cea.hpp"
#include "core_server/internal/ceql/query_transformer/annotate_predicates_with_new_physical_predicates.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/tecs.hpp"
#include "core_server/internal/evaluation/evaluator.hpp"
#include "core_server/internal/evaluation/predicate_evaluator.hpp"
#include "core_server/internal/parsing/ceql_query/parser.hpp"

namespace CORE::Internal::Evaluation::UnitTests {

RingTupleQueue::Tuple add_event(RingTupleQueue::Queue& ring_tuple_queue,
                                uint64_t event_type_id,
                                std::string val1,
                                int64_t val2) {
  uint64_t* data = ring_tuple_queue.start_tuple(event_type_id);
  char* chars = ring_tuple_queue.writer<std::string>(val1.size());
  memcpy(chars, &val1[0], val1.size());
  int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = val2;
  return ring_tuple_queue.get_tuple(data);
}

std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                      std::vector<RingTupleQueue::Tuple>>>
enumerator_to_vector(tECS::Enumerator enumerator) {
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    out;
  for (auto info : enumerator) {
    out.push_back(info);
  }
  return out;
}

std::string output_to_string(tECS::Enumerator enumerator) {
  std::string out = "";
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> info : enumerator) {
    out += "[" + std::to_string(info.first.first) + ","
           + std::to_string(info.first.second) + "]" + ", {";
    for (auto tuple : info.second) {
      RingTupleQueue::Value<std::string_view> val1(tuple[0]);
      RingTupleQueue::Value<std::int64_t> val2(tuple[1]);
      if (tuple.id() == 0) {
        out += "SELL " + std::string(val1.get()) + " "
               + std::to_string(val2.get()) + " ";
      }
    }
    out += "}\n";
  }
  return out;
}

bool is_the_same_as(RingTupleQueue::Tuple tuple,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value) {
  if (tuple.id() != event_type_id) {
    return false;
  }
  std::string_view
    tuple_name = RingTupleQueue::Value<std::string_view>(tuple[0]).get();
  int64_t tuple_val = RingTupleQueue::Value<int64_t>(tuple[1]).get();
  return (tuple_name == name && tuple_val == value);
}

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

  auto visitor = CEQL::FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  Evaluator evaluator(std::move(cea), std::move(tuple_evaluator), 20);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));

  REQUIRE(outputs.size() == 4);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1920));

  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1920));

  REQUIRE(is_the_same_as(outputs[2].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[2].second[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(outputs[2].second[2], 0, "AMZN", 1920));

  REQUIRE(is_the_same_as(outputs[3].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[3].second[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(outputs[3].second[2], 0, "AMZN", 1920));
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

  auto visitor = CEQL::FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea = CEA::DetCEA(std::move(intermediate_cea));

  Evaluator evaluator(std::move(cea), std::move(tuple_evaluator), 5);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(outputs[1].first.first == 0);
  REQUIRE(outputs[1].first.second == 4);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));

  REQUIRE(outputs.size() == 2);

  REQUIRE(outputs[0].first.first == 1);
  REQUIRE(outputs[0].first.second == 6);
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1920));

  REQUIRE(outputs[1].first.first == 1);
  REQUIRE(outputs[1].first.second == 6);
  REQUIRE(is_the_same_as(outputs[1].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[1].second[1], 0, "INTL", 81));
  REQUIRE(is_the_same_as(outputs[1].second[2], 0, "AMZN", 1920));
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

  auto visitor = CEQL::FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  Evaluator evaluator(std::move(cea), std::move(tuple_evaluator), 20);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);
  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 7);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 1);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 7);

  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 8);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 9);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));

  REQUIRE(outputs.size() == 0);
}

TEST_CASE(
  "Evaluation of a query with mix of continuous, contiguous, kleene, and "
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
    "WHERE (SELL)+ as msft: (SELL OR BUY) as intel; SELL as amzn\n"
    "FILTER msft[name='MSFT'] AND msft[price > 100]\n"
    "    AND intel[name='INTL']\n"
    "    AND amzn[name='AMZN'] AND amzn[price < 2000]";

  CEQL::Query query = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer(catalog);
  query = transformer(std::move(query));
  auto predicates = std::move(transformer.physical_predicates);
  auto tuple_evaluator = PredicateEvaluator(std::move(predicates));
  INFO("Tuple Evaluator: " + tuple_evaluator.to_string());

  auto visitor = CEQL::FormulaToLogicalCEA(catalog);
  query.where.formula->accept_visitor(visitor);
  INFO(visitor.current_cea.to_string());
  CEA::CEA intermediate_cea = CEA::CEA(std::move(visitor.current_cea));
  INFO(intermediate_cea.to_string());
  CEA::DetCEA cea(std::move(intermediate_cea));

  Evaluator evaluator(std::move(cea), std::move(tuple_evaluator), 20);

  RingTupleQueue::Tuple tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  auto next_output_enumerator = evaluator.next(tuple, 0);
  auto outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 102);
  next_output_enumerator = evaluator.next(tuple, 1);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 102");
  // 1001101 <- Tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 2);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 1, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 3);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("BUY INTL 80");
  // 1000010 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 4);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 2);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 0);
  REQUIRE(outputs[0].first.second == 4);
  REQUIRE(outputs[1].first.first == 1);
  REQUIRE(outputs[1].first.second == 4);

  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 102));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "MSFT", 101);
  next_output_enumerator = evaluator.next(tuple, 5);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL MSFT 101");
  // 1001101 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "INTL", 80);
  next_output_enumerator = evaluator.next(tuple, 6);
  outputs = enumerator_to_vector(next_output_enumerator);

  INFO("SELL INTL 80");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1900);
  next_output_enumerator = evaluator.next(tuple, 7);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1900");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 1);
  for (std::pair<std::pair<uint64_t, uint64_t>,
                 std::vector<RingTupleQueue::Tuple>> output : outputs) {
  }
  REQUIRE(outputs[0].first.first == 5);
  REQUIRE(outputs[0].first.second == 7);

  REQUIRE(is_the_same_as(outputs[0].second[0], 0, "MSFT", 101));
  REQUIRE(is_the_same_as(outputs[0].second[1], 0, "INTL", 80));
  REQUIRE(is_the_same_as(outputs[0].second[2], 0, "AMZN", 1900));

  tuple = add_event(ring_tuple_queue, 0, "INTL", 81);
  next_output_enumerator = evaluator.next(tuple, 8);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL INTL 81");
  // 1000001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));
  REQUIRE(outputs.size() == 0);

  tuple = add_event(ring_tuple_queue, 0, "AMZN", 1920);
  next_output_enumerator = evaluator.next(tuple, 9);
  outputs = enumerator_to_vector(next_output_enumerator);
  INFO("SELL AMZN 1920");
  // 1101001 <- tuple evaluator
  INFO(output_to_string(next_output_enumerator));

  REQUIRE(outputs.size() == 0);
}
}  // namespace CORE::Internal::Evaluation::UnitTests
