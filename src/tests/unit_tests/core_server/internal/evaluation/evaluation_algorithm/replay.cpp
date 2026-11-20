// The replay decomposition (guides/backlog.md item 29,
// guides/benchmarking/09-replay-decomposition.md) times the automaton by feeding
// it predicate bitsets recorded beforehand, through Evaluator::next_with_bitset,
// instead of letting Evaluator::next evaluate them. That is only valid if the
// two paths produce exactly the same output, event by event. These tests check
// that on real queries and real events, for every strategy this build has:
//   1. predicate evaluation is deterministic: two independent recordings of the
//      same events give identical bitsets, and the baseline and minterm-tree
//      strategies give identical bitsets too;
//   2. replay is faithful: every event's converted output (complex events) is
//      identical on both paths, across windows (WITHIN), consumption policies
//      (CONSUME BY ANY / NONE), and queries with and without output.
// They use core_bench's BenchEvaluator (src/tests/benchmarking/bench_common.hpp),
// so the benchmark harness itself is what gets tested.

// clang-format off
#include "quill/Backend.h"  // must precede the ANTLR headers, see bench_common.hpp
// clang-format on

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "core_server/internal/optimizations/atom_ordering_strategy.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/internal/optimizations/string_equality_strategy.hpp"
#include "shared/datatypes/bitset.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/logging/setup.hpp"
#include "tests/benchmarking/bench_common.hpp"

namespace CORE::Internal::Evaluation::UnitTests::Replay {
namespace {

using Optimizations::AtomOrderingStrategy;
using Optimizations::PredicateEvaluationStrategy;
using Optimizations::StringEqualityStrategy;
namespace Bench = Benchmarking;

// Enough events for the cases below to produce output, small enough for a
// Debug (-O0) build.
constexpr std::size_t kMaxEvents = 20000;

struct Case {
  std::string dataset;
  std::string csv;
  std::string query_dir;
  std::string query;
  bool expects_output;
};

std::string dataset_base(const std::string& dataset) {
  return std::string(CORE_SOURCE_ROOT) + "/src/targets/experiments/" + dataset;
}

// One compiled query plus the events it reads (as the engine routes them).
struct Prepared {
  std::unique_ptr<Bench::BenchEvaluator> engine;
  std::vector<std::shared_ptr<Types::Event>> events;
};

Prepared prepare(const Case& test_case, PredicateEvaluationStrategy strategy) {
  const std::string base = dataset_base(test_case.dataset);
  Prepared prepared;
  prepared.engine = std::make_unique<Bench::BenchEvaluator>();
  Types::StreamInfo stream_info = prepared.engine->declare_stream(
    Bench::read_file(base + "/declaration.core"));
  prepared.engine->add_query(Bench::read_file(base + "/" + test_case.query_dir + "/"
                                              + test_case.query),
                             strategy,
                             AtomOrderingStrategy::AsDiscovered,
                             StringEqualityStrategy::Opaque,
                             /*keep_predicate_copy=*/true);
  for (auto& event :
       Bench::read_events_generic(base + "/" + test_case.csv, stream_info, kMaxEvents)) {
    if (prepared.engine->is_relevant(event->get_event_type_id())) {
      prepared.events.push_back(event);
    }
  }
  return prepared;
}

std::vector<std::string> to_strings(Types::Enumerator& enumerator) {
  std::vector<std::string> out;
  for (const auto& complex_event : enumerator) out.push_back(complex_event.to_string());
  return out;
}

std::vector<PredicateEvaluationStrategy> strategies() {
  std::vector<PredicateEvaluationStrategy> out{PredicateEvaluationStrategy::Default};
  if (Optimizations::is_minterm_tree_available()) {
    out.push_back(PredicateEvaluationStrategy::MintermTree);
  }
  return out;
}

const std::vector<Case>& cases() {
  static const std::vector<Case> all = {
    // WITHIN 10, CONSUME BY ANY, with output.
    {"smart_homes", "smart_homes_data.csv", "queries", "q1_any.txt", true},
    // WITHIN 10, CONSUME BY NONE, with output.
    {"smart_homes", "smart_homes_data.csv", "queries", "q1_none.txt", true},
    // A threshold ladder (several live states per event), with output.
    {"smart_homes",
     "smart_homes_data.csv",
     "queries_optimization",
     "thr_ladder_04.txt",
     true},
    // Repeated equalities: no output, the tree's best case.
    {"smart_homes", "smart_homes_data.csv", "queries_optimization", "rep_eq_04.txt", false},
    // String predicates on another dataset.
    {"taxis", "taxi_data.csv", "queries", "q1_none.txt", false},
  };
  return all;
}

void silence_logging() {
  // The engine fetches the "root" quill logger at construction time.
  Logging::enable_logging_stdout()->set_log_level(quill::LogLevel::None);
}

}  // namespace

TEST_CASE("Replay: predicate bitsets are deterministic and strategy-independent",
          "[Replay]") {
  silence_logging();
  for (const Case& test_case : cases()) {
    INFO(test_case.dataset << "/" << test_case.query);
    std::vector<Bitset> reference;
    for (PredicateEvaluationStrategy strategy : strategies()) {
      Prepared prepared = prepare(test_case, strategy);
      std::vector<Bitset> first = prepared.engine->record_bitsets(prepared.events);
      std::vector<Bitset> second = prepared.engine->record_bitsets(prepared.events);
      REQUIRE(first.size() == prepared.events.size());
      // Two independent evaluations of the same events agree.
      REQUIRE(first == second);
      // Every strategy computes the same bitsets as the first one (baseline).
      if (reference.empty()) {
        reference = std::move(first);
      } else {
        REQUIRE(first == reference);
      }
    }
  }
}

TEST_CASE("Replay: next_with_bitset gives the same output as next, event by event",
          "[Replay]") {
  silence_logging();
  for (const Case& test_case : cases()) {
    for (PredicateEvaluationStrategy strategy : strategies()) {
      INFO(test_case.dataset << "/" << test_case.query << ", strategy "
                             << static_cast<int>(strategy));
      // Two separate engines, so neither path's automaton state can leak into
      // the other's.
      Prepared normal = prepare(test_case, strategy);
      Prepared replay = prepare(test_case, strategy);
      REQUIRE(normal.events.size() == replay.events.size());
      std::vector<Bitset> bitsets = replay.engine->record_bitsets(replay.events);

      std::size_t complex_events = 0;
      for (std::size_t i = 0; i < normal.events.size(); i++) {
        Types::Enumerator expected = normal.engine->process_and_convert(normal.events[i]);
        Types::Enumerator replayed = replay.engine->process_and_convert(replay.events[i],
                                                                        &bitsets[i]);
        std::vector<std::string> expected_text = to_strings(expected);
        std::vector<std::string> replayed_text = to_strings(replayed);
        if (expected_text != replayed_text) {
          INFO("first difference at event " << i);
          REQUIRE(expected_text == replayed_text);
        }
        complex_events += expected_text.size();
      }
      // The cases chosen to produce output really do, so the comparison above
      // is not just empty == empty.
      if (test_case.expects_output) {
        REQUIRE(complex_events > 0);
      }
    }
  }
}

}  // namespace CORE::Internal::Evaluation::UnitTests::Replay
