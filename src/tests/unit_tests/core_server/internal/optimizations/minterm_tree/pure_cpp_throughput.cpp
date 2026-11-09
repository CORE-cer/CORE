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
// Scoped to 3 representative queries, not the full 53, to validate the
// harness before any larger replication:
//   - rep_eq_24 (smart_homes, cheap scalar events)
//   - thr_ladder_24 (smart_homes, cheap scalar events, real per-event work)
//   - rep_like_single_08 (taxis, string-heavy events - the clearest
//     expected gap, since taxis events carry several heap-allocated
//     strings that are expensive to deep-copy)
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

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
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
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"

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
      : backend_(options_for(strategy)) {}

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
                  const std::vector<std::shared_ptr<const Types::Event>>& events) {
    auto start = std::chrono::steady_clock::now();
    for (const auto& event : events) {
      backend_.send_event_to_queries(stream_id, Types::EventWrapper(event));
    }
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  }

 private:
  Interface::Backend<> backend_;
};

Types::StreamInfo declare_smart_homes_stream(ThroughputEngine& engine) {
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
Types::StreamInfo declare_taxis_stream(ThroughputEngine& engine) {
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
std::vector<std::shared_ptr<const Types::Event>>
read_smart_homes_events(const std::string& csv_path, Types::UniqueEventTypeId event_type) {
  std::vector<std::shared_ptr<const Types::Event>> events;
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

    events.push_back(std::make_shared<const Types::Event>(event_type, std::move(attrs)));
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
std::vector<std::shared_ptr<const Types::Event>>
read_taxis_events(const std::string& csv_path, Types::UniqueEventTypeId event_type) {
  std::vector<std::shared_ptr<const Types::Event>> events;
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

    events.push_back(std::make_shared<const Types::Event>(event_type, std::move(attrs)));
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

constexpr int kRepeats = 5;

// Runs one query against one dataset's events for both strategies, appending
// rows to `rows`. Returns nothing; REQUIREs event counts and cross-strategy
// match-count parity before any row is trusted enough to keep.
void run_query(const std::string& dataset_name,
               const std::string& query_name,
               const std::string& query_text,
               const std::vector<std::shared_ptr<const Types::Event>>& events,
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
    std::vector<std::shared_ptr<const Types::Event>>
      events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);

    for (const std::string& name : {"rep_eq_24.txt", "thr_ladder_24.txt"}) {
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
    std::vector<std::shared_ptr<const Types::Event>>
      events = read_taxis_events(base + "/taxi_data.csv", event_type);

    std::string name = "rep_like_single_08.txt";
    std::string query_text = read_file(base + "/queries_optimization/" + name);
    run_query("taxis", name, query_text, events, 585'762, rows);
  }

  std::string out_path = root + "/tests/benchmark/results/pure_cpp_throughput.csv";
  write_csv(out_path, rows);
  std::cout << "Wrote " << rows.size() << " rows to " << out_path << "\n";
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
