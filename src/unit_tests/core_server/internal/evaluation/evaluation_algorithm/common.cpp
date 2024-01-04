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

// Overload of add_event for events with 4 attributes
RingTupleQueue::Tuple add_event(RingTupleQueue::Queue& ring_tuple_queue,
                                uint64_t event_type_id,
                                std::string val1,
                                int64_t val2,
                                int64_t val3) {
  uint64_t* data = ring_tuple_queue.start_tuple(event_type_id);
  char* chars = ring_tuple_queue.writer<std::string>(val1.size());
  memcpy(chars, &val1[0], val1.size());
  int64_t* integer_ptr = ring_tuple_queue.writer<int64_t>();
  *integer_ptr = val2;
  int64_t* integer_ptr_2 = ring_tuple_queue.writer<int64_t>();
  *integer_ptr_2 = val3;

  return ring_tuple_queue.get_tuple(data);
}

std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                      std::vector<RingTupleQueue::Tuple>>>
enumerator_to_vector(tECS::Enumerator& enumerator) {
  enumerator.reset();
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>>
    out;
  for (auto info : enumerator) {
    out.push_back(info);
  }
  return out;
}

std::string output_to_string(tECS::Enumerator& enumerator) {
  enumerator.reset();
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
      if (tuple.id() == 1) {
        out += "BUY " + std::string(val1.get()) + " "
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

// Overload of is_the_same_as for events with 4 attributes
bool is_the_same_as(RingTupleQueue::Tuple tuple,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value,
                    int64_t quantity) {
  if (tuple.id() != event_type_id) {
    return false;
  }
  std::string_view
    tuple_name = RingTupleQueue::Value<std::string_view>(tuple[0]).get();
  int64_t tuple_val = RingTupleQueue::Value<int64_t>(tuple[1]).get();
  int64_t tuple_quantity = RingTupleQueue::Value<int64_t>(tuple[2]).get();
  return (tuple_name == name && tuple_val == value
          && tuple_quantity == quantity);
}

CEQL::FormulaToLogicalCEA
query_to_logical_cea(Catalog& catalog, CEQL::Query& query) {
  CEQL::FormulaToLogicalCEA visitor = CEQL::FormulaToLogicalCEA(catalog);

  query.where.formula->accept_visitor(visitor);
  if (!query.select.is_star) {
    query.select.formula->accept_visitor(visitor);
  }

  return visitor;
}

std::string get_evaluation_info(std::string string_query,
                                CORE::Internal::Catalog catalog,
                                RingTupleQueue::Tuple tuple) {
  CEQL::Query query_test = Parsing::Parser::parse_query(string_query);
  CEQL::AnnotatePredicatesWithNewPhysicalPredicates transformer_test(
    catalog);
  query_test = transformer_test(std::move(query_test));
  auto predicates_test = std::move(transformer_test.physical_predicates);
  auto tuple_evaluator_test = PredicateEvaluator(std::move(predicates_test));
  return tuple_evaluator_test(tuple).get_str(2);
}

std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                      std::vector<RingTupleQueue::Tuple>>>
get_outputs(
  RingTupleQueue::Tuple tuple,
  int current_time,
  Evaluator& evaluator,
  std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                        std::vector<RingTupleQueue::Tuple>>> outputs) {
  bool has_outputs = evaluator.next(tuple, current_time);
  if (has_outputs) {
    auto next_output_enumerator = evaluator.get_enumerator();
    outputs = enumerator_to_vector(next_output_enumerator);
  } else {
    outputs.clear();
  }
  return outputs;
}

}  // namespace CORE::Internal::Evaluation::UnitTests
