#pragma once

// ThroughputEngine measures through the real quarantine pipeline
// (Backend/QuarantineManager/GenericQuery's own worker thread) - and, as
// this investigation found, that means it only ever measured enqueue time,
// not evaluation time (see guides/quarantine-module.md and the dated entry
// in guides/thesis-findings/06-open-questions-and-limits.md, 2026-11-12).
// DirectEvaluator (pure_cpp_throughput.cpp) is the correct way to measure
// evaluation time and should be used for any new work instead. See
// throughput_engine.cpp's own top comment for why ThroughputEngine is kept
// around anyway, and for its "maybe delete" status.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "pure_cpp_throughput_common.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

// One backend, one stream, one query - fresh per (query, strategy, repeat),
// matching compile_vs_processing.py's "one fresh server" convention.
class ThroughputEngine {
 public:
  explicit ThroughputEngine(PredicateEvaluationStrategy strategy)
      : backend_(options_for(strategy)) {
    // Backend<true>'s own constructor calls Logging::enable_logging_stdout()
    // internally (backend.hpp), which *unconditionally* resets "root"'s
    // level back to Debug on every call (setup.cpp:84) - so a one-time
    // suppression before the first ThroughputEngine exists gets undone the
    // moment this constructor runs, every single repeat. Re-suppress here,
    // every time, rather than relying on a level set once from outside.
    Logging::enable_logging_stdout()->set_log_level(quill::LogLevel::None);
  }

  Types::StreamInfo add_stream_type(Types::StreamInfoParsed&& parsed) {
    return backend_.add_stream_type(std::move(parsed));
  }

  // Returns (handler, compile_time_s). Mirrors run_once's "only add_query is
  // timed as compile" in compile_vs_processing.py. out_matches, if given, is
  // forwarded to CountingResultHandler - see its own comment for why.
  std::pair<CountingResultHandler*, double>
  add_query(const std::string& text, size_t* out_matches = nullptr) {
    auto start = std::chrono::steady_clock::now();
    CEQL::Query parsed_query = backend_.parse_sent_query(text);
    auto handler = std::make_unique<CountingResultHandler>(
      QueryCatalog(backend_.get_catalog_reference(), parsed_query), out_matches);
    CountingResultHandler* raw = handler.get();
    backend_.declare_query(std::move(parsed_query), std::move(handler));
    double compile_time_s = std::chrono::duration<double>(std::chrono::steady_clock::now()
                                                          - start)
                              .count();
    return {raw, compile_time_s};
  }

  // Times sending every event, nothing else - no deep copy (events are
  // handed over by reference, not through a pybind boundary), no join
  // afterwards (there is no output-writing thread to join).
  double send_all(Types::StreamTypeId stream_id,
                  const std::vector<std::shared_ptr<Types::Event>>& events) {
    auto start = std::chrono::steady_clock::now();
    for (const auto& event : events) {
      backend_.send_event_to_queries(stream_id, Types::EventWrapper(event));
    }
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  }

 private:
  Interface::Backend<> backend_;
};

// Runs one query against one dataset's events for both strategies, appending
// rows to `rows`. Returns nothing; REQUIREs event counts and cross-strategy
// match-count parity before any row is trusted enough to keep.
inline void run_query(const std::string& dataset_name,
                      const std::string& query_name,
                      const std::string& query_text,
                      const std::vector<std::shared_ptr<Types::Event>>& events,
                      size_t expected_event_count,
                      std::vector<Row>& rows) {
  INFO(dataset_name << " / " << query_name);
  REQUIRE(events.size() == expected_event_count);

  std::optional<size_t> baseline_matches;
  for (auto [strategy, strategy_name] :
       {std::pair{PredicateEvaluationStrategy::Default, std::string("baseline")},
        std::pair{PredicateEvaluationStrategy::MintermTree, std::string("minterm_tree")}}) {
    for (int repeat = 1; repeat <= kRepeats; repeat++) {
      // A fresh engine (and fresh catalog) per repeat, matching
      // compile_vs_processing.py's "one fresh server" convention. Each
      // dataset's event_type id is deterministic across separate catalog
      // instances that declare the same single stream/event type first
      // (the same assumption minterm_tree_path_length.cpp's CompiledQuery
      // already relies on, between its own catalog and the probe catalog
      // events were read against).
      ThroughputEngine engine(strategy);
      Types::StreamInfo stream_info = (dataset_name == "smart_homes")
                                        ? declare_smart_homes_stream(engine)
                                        : declare_taxis_stream(engine);

      auto [handler, compile_time_s] = engine.add_query(query_text);
      double processing_time_s = engine.send_all(stream_info.id, events);

      size_t matches = handler->matches();
      std::string matches_baseline;
      if (strategy_name == "baseline") {
        if (repeat == 1) baseline_matches = matches;
      } else if (repeat == 1) {
        REQUIRE(baseline_matches.has_value());
        // Exact equality for the overwhelming majority of this suite (0
        // matches - unsatisfiable guards, rep_eq's household_id=99, etc.),
        // but a generous relative tolerance for queries with many real
        // matches: thr_ladder_08's own baseline match count already varies
        // repeat-to-repeat with no strategy change at all (confirmed
        // directly, 2026-11-11: 2547-2602 across 15 baseline repeats, mean
        // 2563, ~2.1% spread) - automaton tie-breaking among same-timestamp
        // events, not a bug, so exact equality is the wrong bar for these.
        // See guides/thesis-findings/06-open-questions-and-limits.md.
        size_t diff = (matches > *baseline_matches) ? (matches - *baseline_matches)
                                                    : (*baseline_matches - matches);
        bool close_enough = (*baseline_matches == 0)
                              ? (matches == 0)
                              : (static_cast<double>(diff)
                                   / static_cast<double>(*baseline_matches)
                                 <= 0.10);
        matches_baseline = close_enough ? "True" : "False";
        INFO("baseline matches=" << *baseline_matches
                                 << ", this strategy matches=" << matches);
        REQUIRE(matches_baseline == "True");
      }

      rows.push_back({dataset_name,
                      query_name,
                      strategy_name,
                      repeat,
                      events.size(),
                      compile_time_s,
                      processing_time_s,
                      matches_baseline});
      std::cout << dataset_name << "/" << query_name << " [" << strategy_name
                << "] repeat " << repeat << ": compile " << compile_time_s
                << "s, processing " << processing_time_s << "s, matches " << matches
                << "\n";
    }
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
