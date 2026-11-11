// A pure-C++ throughput harness: drives Interface::Backend<> directly, the
// way optimized_engine_parity.cpp already does for correctness, so that
// timing never crosses the pybind boundary. compile_vs_processing.py's
// processing_time_s times send_events (which deep-copies every event inside
// the timed region to break the nanobind shared_ptr tie, see
// src/pybind/python_binding.cpp:162-170) plus get_output (which joins
// worker threads to drain pending output text). Neither cost exists here:
// Backend owns no threads of its own (see backend.hpp), and events are
// handed over by reference, not copied. The gap between this harness's
// processing_time_s and compile_vs_processing.py's, for the same query and
// events, is a direct measurement of that pybind-boundary overhead - see
// guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09).
//
// Scoped to a handful of representative queries, not the full 53, to
// validate the harness before any larger replication:
//   - floor_01 (both datasets - the same single-alias, single-predicate
//     floor compile_vs_processing.py uses, so above_floor_summary.py can
//     do its usual same-session floor subtraction on this CSV too)
//   - rep_eq_24 (smart_homes, cheap scalar events, the project's best
//     documented win: 24 identical atoms collapsing to 2 leaves)
//   - thr_ladder_24 (smart_homes, cheap scalar events, real per-event work)
//   - rep_arith_int_single_{08,24} (smart_homes, backlog item 19: the same
//     2-leaf collapse as rep_eq, but each atom is a 20-term int64_t
//     addition chain instead of a bare comparison - stays inside Z3's
//     modeled subset, unlike item 16's accidentally-opaque double
//     arithmetic, see physical_predicate_z3_translator.hpp:51-58)
//   - rep_like_single_08 (taxis, string-heavy events - the clearest
//     expected gap, since taxis events carry several heap-allocated
//     strings that are expensive to deep-copy)
//
// 15 repeats per (query, strategy), matching the project's standard
// (tests/benchmark/above_floor_summary.py's own docstring).
//
// Writes a CSV in compile_vs_processing.py's own schema
// (dataset,query,strategy,repeat,event_count,compile_time_s,
// processing_time_s,matched_expected,matches_baseline,output_bytes) to
// tests/benchmark/results/, so existing tooling can load it; matched_expected
// is always blank (no expected-output file is used here) and output_bytes is
// always 0 (no text output is ever formatted - see CountingResultHandler
// below), both already-valid values in that schema (see any floor_01.txt
// row, which matches nothing and reports output_bytes 0 too).
//
// Correctness, checked before any timing is trusted: each query's event
// count must match the dataset's known row count, and baseline's and
// minterm_tree's match counts must agree (tree-vs-baseline correctness is
// already exhaustively checked elsewhere by the digest-based benchmark
// pipeline and by test_support.hpp's ParityChecker; this just confirms the
// new hand-rolled readers and query wiring didn't drop or miscount events).
//
// Hidden tag, like minterm_tree_path_length.cpp - reads full multi-hundred-
// thousand/million-row datasets and must not run routinely. Needs -o (the
// minterm_tree strategy requires Z3):
//   ./build/Debug-opt/tests "[PureCppThroughput]"

// quill/Backend.h (for quill::Backend::stop()) must come before any header
// that pulls in the ANTLR runtime (core_server/internal/ceql/query/query.hpp,
// below): ANTLR's Recognizer.h #undefs the standard EOF macro to define its
// own CEQLQueryLexer::EOF member, and once that happens earlier in this
// translation unit, quill's bundled fmt headers (which need the real EOF
// macro from <cstdio>) fail to compile. setup.hpp only pulls in
// quill/Logger.h, not this - python_binding.cpp includes it separately for
// the same quill::Backend::stop() call.
// clang-format off
#include "quill/Backend.h"
// clang-format on

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "core_server/internal/ceql/query/query.hpp"
#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/interface/backend.hpp"
#include "core_server/internal/interface/engine_options.hpp"
#include "core_server/internal/optimizations/optimized_predicate_evaluator_factory.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "core_server/library/components/stream_listeners/offline/offline_streams_listener.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/stream.hpp"
#include "shared/datatypes/value.hpp"
#include "shared/logging/setup.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

Interface::EngineOptions options_for(PredicateEvaluationStrategy strategy) {
  Interface::EngineOptions options;
  options.predicate_evaluation = strategy;
  return options;
}

// Tallies total calls and total matches with a plain counter instead of
// DirectOutputTestResultHandler's mutex/condvar handshake (common.hpp):
// Backend has no worker threads of its own (backend.hpp), so in this
// offline, single-threaded usage handle_complex_event runs synchronously
// within send_event_to_queries's own call stack, and nothing needs to wait
// for anything. No text is ever formatted, matching the "no output
// overhead" half of what this harness is trying to isolate.
class CountingResultHandler : public Library::Components::ResultHandler {
  size_t matches_ = 0;

 public:
  explicit CountingResultHandler(const QueryCatalog&)
      : Library::Components::ResultHandler(
          Library::Components::ResultHandlerType::CUSTOM) {}

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {
    if (!internal_enumerator.has_value()) return;
    Types::Enumerator enumerator = get_query_catalog().convert_enumerator(
      std::move(internal_enumerator.value()));
    matches_ += enumerator.complex_events.size();
  }

  void start() override {}

  std::string get_identifier() const override { return "CountingResultHandler"; }

  size_t matches() const { return matches_; }
};

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
  // timed as compile" in compile_vs_processing.py.
  std::pair<CountingResultHandler*, double> add_query(const std::string& text) {
    auto start = std::chrono::steady_clock::now();
    CEQL::Query parsed_query = backend_.parse_sent_query(text);
    auto handler = std::make_unique<CountingResultHandler>(
      QueryCatalog(backend_.get_catalog_reference(), parsed_query));
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

// Drives the engine the same way OfflineServer actually does (server.hpp:32-63),
// minus the Router/ZMQ pieces that only matter for the client-command protocol,
// not event ingestion: a Backend<false> (the real server's NoLogging=false - see
// backend.hpp:50-52 - which leaves the shared "root" Quill logger at whatever
// enable_logging_rotating() below sets it to, TraceL3, instead of ThroughputEngine's
// Backend<> = Backend<true>, which sets it to Debug via enable_logging_stdout()),
// and OfflineStreamsListener (offline_streams_listener.hpp), which ThroughputEngine's
// send_all bypasses entirely. Exists to test whether that difference - not pybind,
// not CPU pinning, both already ruled out - explains why this harness's rep_eq_24
// result contradicts the Python-measured one (see
// guides/thesis-findings/06-open-questions-and-limits.md, 2026-11-09).
class ListenerEngine {
 public:
  explicit ListenerEngine(PredicateEvaluationStrategy strategy)
      : backend_(std::in_place, options_for(strategy)) {
    listener_.emplace(*backend_, backend_mutex_);
  }

  Types::StreamInfo add_stream_type(Types::StreamInfoParsed&& parsed) {
    return backend_->add_stream_type(std::move(parsed));
  }

  std::pair<CountingResultHandler*, double> add_query(const std::string& text) {
    auto start = std::chrono::steady_clock::now();
    CEQL::Query parsed_query = backend_->parse_sent_query(text);
    auto handler = std::make_unique<CountingResultHandler>(
      QueryCatalog(backend_->get_catalog_reference(), parsed_query));
    CountingResultHandler* raw = handler.get();
    backend_->declare_query(std::move(parsed_query), std::move(handler));
    double compile_time_s = std::chrono::duration<double>(std::chrono::steady_clock::now()
                                                          - start)
                              .count();
    return {raw, compile_time_s};
  }

  // One Stream per event, matching send_events's own per-event receive_stream
  // call (python_binding.cpp:162-170) exactly - not batched into one Stream
  // for all events, which is not what the real path ever does. Then tears
  // the backend down and stops Quill's backend thread INSIDE the timed
  // region, matching get_output()'s own per-repeat teardown
  // (python_binding.cpp:172-186) exactly - without this, nothing ever drains
  // Quill's queue across repeats in this harness (unlike the real path,
  // which rebuilds a fresh server and calls get_output() every single
  // repeat), which the first attempt at this found was confounding the
  // result with an accumulating backlog rather than measuring the
  // logging-path difference cleanly (see
  // guides/thesis-findings/06-open-questions-and-limits.md, 2026-11-09).
  // `handler` must be read before teardown: it's owned by backend_'s own
  // query storage, so it's destroyed along with backend_.reset() below.
  //
  // Timed in two separate phases rather than one, to show *where* the time
  // actually goes: Quill's backend thread drains its queue concurrently
  // while events are sent (confirmed from quill/backend/BackendWorker.h -
  // UnboundedSPSCQueue.h's production is wait-free, so a fast producer never
  // blocks, it just outruns the consumer), but Backend::stop() joins that
  // thread, which first runs _exit()'s while(true) drain-everything loop
  // (BackendWorker.h:408-436) before returning - so whatever backlog never
  // got drained concurrently during the loop gets fully, synchronously
  // processed here instead. If that's the real mechanism, baseline's loop
  // time should stay close to its direct-path number while its teardown
  // time absorbs the rest; minterm_tree's split should be far less lopsided,
  // since its slower per-event cost gives the backend thread more real time
  // to keep up concurrently. See
  // guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09).
  std::pair<double, size_t>
  send_all_and_teardown(Types::StreamTypeId stream_id,
                        const std::vector<std::shared_ptr<Types::Event>>& events,
                        CountingResultHandler* handler) {
    auto loop_start = std::chrono::steady_clock::now();
    for (const auto& event : events) {
      listener_->receive_stream(Types::Stream{stream_id, {event}});
    }
    double loop_time_s = std::chrono::duration<double>(std::chrono::steady_clock::now()
                                                       - loop_start)
                           .count();

    size_t matches = handler->matches();

    auto teardown_start = std::chrono::steady_clock::now();
    // listener_ only holds references to backend_/backend_mutex_, so
    // dropping it first, then backend_, is always safe - the reverse order
    // would leave backend_ dangling while listener_ still exists.
    listener_.reset();
    backend_.reset();
    quill::Backend::stop();
    double teardown_time_s = std::chrono::duration<double>(
                               std::chrono::steady_clock::now() - teardown_start)
                               .count();

    std::cout << "  loop=" << loop_time_s << "s  teardown=" << teardown_time_s << "s\n";
    return {loop_time_s + teardown_time_s, matches};
  }

 private:
  // Member order matters: OfflineServer builds its logger before its backend
  // (server.hpp:35-39), so "root" is already at TraceL3 by the time Backend's
  // own constructor calls get_logger("root"). Declaration order here mirrors
  // that. backend_/listener_ are optional so send_all_and_teardown can
  // destroy them mid-repeat, before the next repeat's constructor rebuilds
  // them from scratch - matching a fresh PyOfflineServer every repeat.
  quill::Logger* logger_ = Logging::enable_logging_rotating();
  std::optional<Interface::Backend<false>> backend_;
  std::mutex backend_mutex_;
  std::optional<Library::Components::OfflineStreamsListener> listener_;
};

template <class Engine>
Types::StreamInfo declare_smart_homes_stream(Engine& engine) {
  std::vector<Types::AttributeInfo> load_attrs;
  load_attrs.emplace_back("id", Types::ValueTypes::INT64);
  load_attrs.emplace_back("plug_timestamp", Types::ValueTypes::INT64);
  load_attrs.emplace_back("value", Types::ValueTypes::DOUBLE);
  load_attrs.emplace_back("plug_id", Types::ValueTypes::INT64);
  load_attrs.emplace_back("household_id", Types::ValueTypes::INT64);
  return engine.add_stream_type({"S", {{"LOAD", std::move(load_attrs)}}});
}

// The real taxis schema (declaration.core), reproduced here the same way
// minterm_tree_path_length.cpp reproduces smart_homes's: it's short and
// changing it unnoticed here would be a bigger problem than this duplication.
template <class Engine>
Types::StreamInfo declare_taxis_stream(Engine& engine) {
  std::vector<Types::AttributeInfo> trip_attrs;
  trip_attrs.emplace_back("id", Types::ValueTypes::INT64);
  trip_attrs.emplace_back("medallion", Types::ValueTypes::STRING_VIEW);
  trip_attrs.emplace_back("hack_license", Types::ValueTypes::STRING_VIEW);
  trip_attrs.emplace_back("pickup_datetime", Types::ValueTypes::INT64);
  trip_attrs.emplace_back("dropoff_datetime", Types::ValueTypes::INT64);
  trip_attrs.emplace_back("long_time_in_secs", Types::ValueTypes::INT64);
  trip_attrs.emplace_back("trip_distance", Types::ValueTypes::DOUBLE);
  trip_attrs.emplace_back("pickup_zone", Types::ValueTypes::STRING_VIEW);
  trip_attrs.emplace_back("dropoff_zone", Types::ValueTypes::STRING_VIEW);
  trip_attrs.emplace_back("payment_type", Types::ValueTypes::STRING_VIEW);
  trip_attrs.emplace_back("fare_amount", Types::ValueTypes::DOUBLE);
  trip_attrs.emplace_back("surcharge", Types::ValueTypes::DOUBLE);
  trip_attrs.emplace_back("mta_tax", Types::ValueTypes::DOUBLE);
  trip_attrs.emplace_back("tip_amount", Types::ValueTypes::DOUBLE);
  trip_attrs.emplace_back("tolls_amount", Types::ValueTypes::DOUBLE);
  trip_attrs.emplace_back("total_amount", Types::ValueTypes::DOUBLE);
  return engine.add_stream_type({"S", {{"TRIP", std::move(trip_attrs)}}});
}

std::string read_file(const std::string& path) {
  std::ifstream in(path);
  REQUIRE(in.is_open());
  std::ostringstream buf;
  buf << in.rdbuf();
  return buf.str();
}

// smart_homes_data.csv's fixed format:
// event_type,id,plug_timestamp,value,plug_id,household_id.
std::vector<std::shared_ptr<Types::Event>>
read_smart_homes_events(const std::string& csv_path, Types::UniqueEventTypeId event_type) {
  std::vector<std::shared_ptr<Types::Event>> events;
  std::ifstream in(csv_path);
  REQUIRE(in.is_open());
  std::string line;
  std::getline(in, line);  // header
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    std::stringstream ss(line);
    std::string event_type_name, id_s, plug_timestamp_s, value_s, plug_id_s,
      household_id_s;
    std::getline(ss, event_type_name, ',');
    std::getline(ss, id_s, ',');
    std::getline(ss, plug_timestamp_s, ',');
    std::getline(ss, value_s, ',');
    std::getline(ss, plug_id_s, ',');
    std::getline(ss, household_id_s, ',');

    std::vector<std::unique_ptr<Types::Value>> attrs;
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(id_s)));
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(plug_timestamp_s)));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(value_s)));
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(plug_id_s)));
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(household_id_s)));

    events.push_back(std::make_shared<Types::Event>(event_type, std::move(attrs)));
  }
  return events;
}

// taxi_data.csv's fixed format: t,id,medallion,hack_license,pickup_datetime,
// dropoff_datetime,trip_time_in_secs,trip_distance,pickup_zone,dropoff_zone,
// payment_type,fare_amount,surcharge,mta_tax,tip_amount,tolls_amount,
// total_amount. No general CSV parser exists in C++ in this repo (only
// Python's tests/e2e/csv_parser.py); none of these fields contain commas
// (checked directly against the real file), so a plain split is enough,
// the same approach read_smart_homes_events already takes.
std::vector<std::shared_ptr<Types::Event>>
read_taxis_events(const std::string& csv_path, Types::UniqueEventTypeId event_type) {
  std::vector<std::shared_ptr<Types::Event>> events;
  std::ifstream in(csv_path);
  REQUIRE(in.is_open());
  std::string line;
  std::getline(in, line);  // header
  while (std::getline(in, line)) {
    if (line.empty()) continue;
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;
    while (std::getline(ss, field, ',')) fields.push_back(field);
    REQUIRE(fields.size() == 17);

    std::vector<std::unique_ptr<Types::Value>> attrs;
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(fields[1])));
    attrs.push_back(std::make_unique<Types::StringValue>(fields[2]));
    attrs.push_back(std::make_unique<Types::StringValue>(fields[3]));
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(fields[4])));
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(fields[5])));
    attrs.push_back(std::make_unique<Types::IntValue>(std::stoll(fields[6])));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(fields[7])));
    attrs.push_back(std::make_unique<Types::StringValue>(fields[8]));
    attrs.push_back(std::make_unique<Types::StringValue>(fields[9]));
    attrs.push_back(std::make_unique<Types::StringValue>(fields[10]));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(fields[11])));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(fields[12])));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(fields[13])));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(fields[14])));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(fields[15])));
    attrs.push_back(std::make_unique<Types::DoubleValue>(std::stod(fields[16])));

    events.push_back(std::make_shared<Types::Event>(event_type, std::move(attrs)));
  }
  return events;
}

struct Row {
  std::string dataset;
  std::string query;
  std::string strategy;
  int repeat;
  size_t event_count;
  double compile_time_s;
  double processing_time_s;
  std::string matches_baseline;  // "True" / "False" / "" (blank = not applicable)
};

void write_csv(const std::string& path, const std::vector<Row>& rows) {
  std::ofstream out(path);
  REQUIRE(out.is_open());
  out << "dataset,query,strategy,repeat,event_count,compile_time_s,"
         "processing_time_s,matched_expected,matches_baseline,output_bytes\n";
  for (const Row& row : rows) {
    out << row.dataset << "," << row.query << "," << row.strategy << "," << row.repeat
        << "," << row.event_count << "," << row.compile_time_s << ","
        << row.processing_time_s << "," << "" << "," << row.matches_baseline << "," << 0
        << "\n";
  }
}

constexpr int kRepeats = 15;

// Runs one query against one dataset's events for both strategies, appending
// rows to `rows`. Returns nothing; REQUIREs event counts and cross-strategy
// match-count parity before any row is trusted enough to keep.
void run_query(const std::string& dataset_name,
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

// Same shape as run_query, but through ListenerEngine (Backend<false> +
// OfflineStreamsListener) instead of ThroughputEngine - see ListenerEngine's
// own comment for why. Strategy names get a "_via_listener" suffix so the
// output CSV stays self-describing next to run_query's rows.
void run_query_via_listener(const std::string& dataset_name,
                            const std::string& query_name,
                            const std::string& query_text,
                            const std::vector<std::shared_ptr<Types::Event>>& events,
                            size_t expected_event_count,
                            std::vector<Row>& rows) {
  INFO(dataset_name << " / " << query_name << " (via listener)");
  REQUIRE(events.size() == expected_event_count);

  // Lets a single strategy run alone (e.g. under its own `perf stat`
  // invocation, matching how backlog item 17 already profiled rep_like_single
  // one strategy at a time - guides/backlog.md): CORE_LISTENER_STRATEGY_FILTER
  // set to "baseline" or "minterm_tree" skips the other; unset runs both.
  const char* strategy_filter = std::getenv("CORE_LISTENER_STRATEGY_FILTER");

  std::optional<size_t> baseline_matches;
  for (auto [strategy, strategy_name] :
       {std::pair{PredicateEvaluationStrategy::Default,
                  std::string("baseline_via_listener")},
        std::pair{PredicateEvaluationStrategy::MintermTree,
                  std::string("minterm_tree_via_listener")}}) {
    if (strategy_filter != nullptr && strategy_name.rfind(strategy_filter, 0) != 0) {
      continue;
    }
    for (int repeat = 1; repeat <= kRepeats; repeat++) {
      ListenerEngine engine(strategy);
      Types::StreamInfo stream_info = (dataset_name == "smart_homes")
                                        ? declare_smart_homes_stream(engine)
                                        : declare_taxis_stream(engine);

      auto [handler, compile_time_s] = engine.add_query(query_text);
      auto [processing_time_s,
            matches] = engine.send_all_and_teardown(stream_info.id, events, handler);
      // handler is a dangling pointer from here on (destroyed inside
      // send_all_and_teardown along with backend_) - never dereferenced again.

      std::string matches_baseline;
      if (strategy_name == "baseline_via_listener") {
        if (repeat == 1) baseline_matches = matches;
      } else if (repeat == 1 && baseline_matches.has_value()) {
        // Only checked when baseline actually ran in this invocation -
        // CORE_LISTENER_STRATEGY_FILTER can scope a run to minterm_tree
        // alone, with no baseline to compare against.
        matches_baseline = (matches == *baseline_matches) ? "True" : "False";
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

}  // namespace

TEST_CASE("Pure-C++ throughput: engine cost without the pybind boundary",
          "[.][PureCppThroughput]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  std::vector<Row> rows;

  {
    const std::string base = root + "/src/targets/experiments/smart_homes";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    INFO("Parsing the real smart_homes dataset once for both queries below");
    std::vector<std::shared_ptr<Types::Event>>
      events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

    for (const std::string& name : {"floor_01.txt",
                                    "rep_eq_24.txt",
                                    "thr_ladder_24.txt",
                                    "rep_arith_int_single_08.txt",
                                    "rep_arith_int_single_24.txt"}) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query("smart_homes", name, query_text, events, 1'000'000, rows);
    }
  }

  {
    const std::string base = root + "/src/targets/experiments/taxis";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_taxis_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    INFO("Parsing the real taxis dataset once");
    std::vector<std::shared_ptr<Types::Event>>
      events = read_taxis_events(base + "/taxi_data.csv", event_type);

    for (const std::string& name : {"floor_01.txt", "rep_like_single_08.txt"}) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query("taxis", name, query_text, events, 585'762, rows);
    }
  }

  std::string out_path = root + "/tests/benchmark/results/pure_cpp_throughput.csv";
  write_csv(out_path, rows);
  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

// Tests the logging/listener-path lead directly: does routing through
// Backend<false> + OfflineStreamsListener, exactly like OfflineServer does,
// change rep_eq_24's result? See ListenerEngine's comment and
// guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09) for the
// full reasoning. Scoped to rep_eq_24 only - the sharpest instance of the
// contradiction, not a full re-sweep.
TEST_CASE("Pure-C++ throughput via the real listener path: does it change rep_eq_24?",
          "[.][PureCppThroughputViaListener]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }

  const std::string root = CORE_SOURCE_ROOT;
  std::vector<Row> rows;

  const std::string base = root + "/src/targets/experiments/smart_homes";
  ThroughputEngine probe(PredicateEvaluationStrategy::Default);
  Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
  Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

  INFO("Parsing the real smart_homes dataset once");
  std::vector<std::shared_ptr<Types::Event>>
    events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

  std::string query_text = read_file(base + "/queries_optimization/rep_eq_24.txt");
  run_query_via_listener("smart_homes",
                         "rep_eq_24.txt",
                         query_text,
                         events,
                         1'000'000,
                         rows);

  std::string out_path = root
                         + "/tests/benchmark/results/"
                           "pure_cpp_throughput_via_listener.csv";
  write_csv(out_path, rows);
  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

// Every query file this project has ever built for smart_homes/taxis
// (src/targets/experiments/{smart_homes,taxis}/queries_optimization/ -
// listed directly with `ls`, not reconstructed from memory), through the
// direct ThroughputEngine path only (no ListenerEngine, no Backend<false>,
// no teardown) - the path never implicated in any of the logging-backlog
// findings above. Logging is suppressed to the practical minimum (set to
// LogLevel::None, once, before any engine exists) rather than merely
// filtered: with 63 queries x 2 strategies x 15 repeats, even the
// Debug-level LOG_INFO per add_query (the full query text, plus the tree-
// shape line) is real, repeated stdout-writing overhead, same lesson as
// the very first round's unexpectedly slow wall-clock time. Tree-vs-
// baseline parity is checked the same cheap way every round has (handled
// inside run_query) - correctness itself is treated as already settled by
// now, not re-verified here via tree-shape logs (which logging being off
// makes unavailable anyway). See
// guides/thesis-findings/06-open-questions-and-limits.md (2026-11-09).
TEST_CASE("Pure-C++ throughput: full rundown of every evaluated query",
          "[.][PureCppFullRundown]") {
  if (!is_minterm_tree_available()) {
    SKIP("The minterm-tree optimization is not built into this configuration");
  }
  // Logging is suppressed per-repeat inside ThroughputEngine's own
  // constructor (see its comment) - that's the fix that actually sticks,
  // not a one-time call here.

  const std::string root = CORE_SOURCE_ROOT;
  std::vector<Row> rows;
  // Written after every query, not just once at the end: this is a long
  // (multi-hour) unattended run, and a crash partway through should leave
  // whatever's already measured on disk, not nothing - this round already
  // lost one run to a closed window with no partial save to show for it.
  std::string out_path = root + "/tests/benchmark/results/pure_cpp_full_rundown.csv";

  {
    const std::string base = root + "/src/targets/experiments/smart_homes";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_smart_homes_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    std::vector<std::shared_ptr<Types::Event>>
      events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

    // Full list (2026-11-11), 50 of the 51 real query files in this
    // directory - everything except thr_ladder_08.
    //
    // late_exit_thr_{08,12,16} are included: an earlier attempt excluded
    // them after 30+ minutes of apparently frozen output, but that was a
    // stdout-buffering illusion (redirected, non-TTY output is fully
    // buffered, so a long-running process's progress can look completely
    // frozen for 20-30+ min while real work continues underneath) - not a
    // genuine hang. Confirmed by watching the same illusion happen and
    // resolve on rep_arith_int_single_08 later in the same run, and by
    // re-running late_exit_thr_* with `stdbuf -oL` (line-buffered) for
    // reliable monitoring: all three completed cleanly, no issues.
    //
    // thr_ladder_08 is excluded for a different and more serious reason: a
    // GENUINE parity divergence, not noise or a tolerance edge case -
    // baseline=2496 matches, minterm_tree=2999 matches on its first repeat
    // (~20% relative difference, confirmed reproducible across two
    // independent runs with different absolute counts both times). This is
    // far beyond the ~2% repeat-to-repeat jitter baseline alone shows for
    // this query (confirmed directly, see the parity-tolerance comment
    // below). Needs dedicated investigation - excluded because it's a
    // real, unresolved correctness question a timing benchmark shouldn't
    // paper over with a looser tolerance, not because it's inconvenient.
    // See guides/thesis-findings/06-open-questions-and-limits.md
    // (2026-11-11) - read that entry before re-including this query.
    const std::vector<std::string> names = {
      "control_eq_01.txt",
      "control_eq_01_guard.txt",
      "control_thr_01.txt",
      "control_thr_01_guard.txt",
      "eq_ladder_04.txt",
      "eq_ladder_04_guard.txt",
      "eq_ladder_08.txt",
      "eq_ladder_08_guard.txt",
      "eq_ladder_16.txt",
      "eq_ladder_16_guard.txt",
      "floor_01.txt",
      "late_exit_thr_08.txt",
      "late_exit_thr_12.txt",
      "late_exit_thr_16.txt",
      "mix_grouped_08.txt",
      "mix_grouped_08_guard.txt",
      "mix_grouped_16.txt",
      "mix_grouped_16_guard.txt",
      "mix_interleaved_08.txt",
      "mix_interleaved_08_guard.txt",
      "mix_interleaved_16.txt",
      "mix_interleaved_16_guard.txt",
      "rep_arith_int_single_08.txt",
      "rep_arith_int_single_24.txt",
      "rep_arith_single_03.txt",
      "rep_arith_single_04.txt",
      "rep_arith_single_08.txt",
      "rep_arith_single_16.txt",
      "rep_arith_single_24.txt",
      "rep_eq_03.txt",
      "rep_eq_04.txt",
      "rep_eq_08.txt",
      "rep_eq_16.txt",
      "rep_eq_24.txt",
      "rep_eq_single_03.txt",
      "rep_eq_single_04.txt",
      "rep_eq_single_08.txt",
      "rep_eq_single_16.txt",
      "rep_eq_single_24.txt",
      "thr_ladder_04.txt",
      "thr_ladder_04_guard.txt",
      // thr_ladder_08.txt excluded - see comment above.
      "thr_ladder_08_guard.txt",
      "thr_ladder_16.txt",
      "thr_ladder_16_guard.txt",
      "thr_ladder_24.txt",
      "thr_ladder_24_guard.txt",
      "worst_mix_03.txt",
      "worst_mix_08.txt",
      "worst_mix_16.txt",
      "worst_mix_24.txt",
    };
    for (const std::string& name : names) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query("smart_homes", name, query_text, events, 1'000'000, rows);
      write_csv(out_path, rows);
    }
  }

  {
    const std::string base = root + "/src/targets/experiments/taxis";
    ThroughputEngine probe(PredicateEvaluationStrategy::Default);
    Types::StreamInfo probe_stream = declare_taxis_stream(probe);
    Types::UniqueEventTypeId event_type = probe_stream.events_info.at(0).id;

    std::vector<std::shared_ptr<Types::Event>>
      events = read_taxis_events(base + "/taxi_data.csv", event_type);

    const std::vector<std::string> names = {
      "floor_01.txt",
      "rep_like_single_03.txt",
      "rep_like_single_04.txt",
      "rep_like_single_08.txt",
      "rep_like_single_12.txt",
      "rep_like_single_16.txt",
      "str_eq_ladder_04.txt",
      "str_eq_ladder_08.txt",
      "str_eq_ladder_16.txt",
      "str_eq_ladder_24.txt",
      "str_rep_eq_04.txt",
      "str_rep_eq_08.txt",
      "str_rep_eq_16.txt",
      "str_rep_eq_24.txt",
    };
    for (const std::string& name : names) {
      std::string query_text = read_file(base + "/queries_optimization/" + name);
      run_query("taxis", name, query_text, events, 585'762, rows);
      write_csv(out_path, rows);
    }
  }

  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
