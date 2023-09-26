#pragma once

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
                                int64_t val2);

RingTupleQueue::Tuple add_event(RingTupleQueue::Queue& ring_tuple_queue,
                                uint64_t event_type_id,
                                std::string val1,
                                int64_t val2,
                                int64_t val3);

std::vector<std::pair<std::pair<uint64_t, uint64_t>,
                      std::vector<RingTupleQueue::Tuple>>>
enumerator_to_vector(tECS::Enumerator& enumerator);

std::string output_to_string(tECS::Enumerator& enumerator);

bool is_the_same_as(RingTupleQueue::Tuple tuple,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value);

bool is_the_same_as(RingTupleQueue::Tuple tuple,
                    uint64_t event_type_id,
                    std::string name,
                    int64_t value,
                    int64_t quantity);

CEQL::FormulaToLogicalCEA
query_to_logical_cea(Catalog& catalog, CEQL::Query& query);
std::string get_evaluation_info(std::string string_query, CORE::Internal::Catalog catalog, RingTupleQueue::Tuple tuple);
}  // namespace CORE::Internal::Evaluation::UnitTests
