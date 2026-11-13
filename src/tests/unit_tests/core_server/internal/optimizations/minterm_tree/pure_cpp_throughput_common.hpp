#pragma once

// Shared helpers for this directory's pure-C++ (no pybind) measurement
// harnesses: dataset readers/declarers, the CSV row/writer, the shared
// result handler, and small option-building helpers. Used by both
// pure_cpp_throughput.cpp (DirectEvaluator - the correct way to measure
// evaluation time) and throughput_engine.cpp/throughput_engine.hpp (the
// quarantine-bound ThroughputEngine/ListenerEngine paths - see
// throughput_engine.cpp's own top comment for their status).

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/enumeration/tecs/enumerator.hpp"
#include "core_server/internal/interface/engine_options.hpp"
#include "core_server/internal/optimizations/predicate_evaluation_strategy.hpp"
#include "core_server/library/components/result_handler/result_handler.hpp"
#include "core_server/library/components/result_handler/result_handler_types.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/enumerator.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/value.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

inline Interface::EngineOptions options_for(PredicateEvaluationStrategy strategy) {
  Interface::EngineOptions options;
  options.predicate_evaluation = strategy;
  return options;
}

constexpr int kRepeats = 15;

// Tallies total calls and total matches with a plain counter instead of
// DirectOutputTestResultHandler's mutex/condvar handshake (common.hpp).
// NOTE: handle_complex_event does NOT run synchronously within
// send_event_to_queries's call stack for ThroughputEngine/run_query
// (throughput_engine.hpp) - see the quarantine pipeline explained in
// DirectEvaluator's own comment (pure_cpp_throughput.cpp) and in
// guides/quarantine-module.md: DirectPolicy only enqueues on the calling
// thread, and this handler is actually invoked later, from GenericQuery's
// own worker thread. It works correctly either way (it only tallies a
// count), but callers must not assume its count is up to date the instant
// send_all returns unless they went through DirectEvaluator instead, which
// does call it synchronously, on the calling thread, with no queues or
// worker threads involved. No text is ever formatted, matching the "no
// output overhead" half of what these harnesses are trying to isolate.
class CountingResultHandler : public Library::Components::ResultHandler {
  size_t matches_ = 0;
  // Optional: written on every match, in addition to matches_. Lets a
  // caller read the final count from a variable it owns after the engine
  // that owns this handler has been torn down (which destroys this handler
  // along with it) - the only race-free way to read a count from the real
  // quarantine-bound path, since nothing else here waits for that path's
  // worker thread to finish draining before the handler itself would
  // otherwise be read. Safe with no atomics: the writer (this handler, on
  // the query's own worker thread) and the reader (main thread, after the
  // owning engine's destructor joins that worker thread) are never
  // concurrent - std::thread::join() is itself a synchronizes-with edge.
  size_t* out_matches_ = nullptr;

 public:
  explicit CountingResultHandler(const QueryCatalog&, size_t* out_matches = nullptr)
      : Library::Components::ResultHandler(Library::Components::ResultHandlerType::CUSTOM),
        out_matches_(out_matches) {}

  void handle_complex_event(
    std::optional<Internal::tECS::Enumerator>&& internal_enumerator) override {
    if (!internal_enumerator.has_value()) return;
    Types::Enumerator enumerator = get_query_catalog().convert_enumerator(
      std::move(internal_enumerator.value()));
    matches_ += enumerator.complex_events.size();
    if (out_matches_ != nullptr) *out_matches_ = matches_;
  }

  void start() override {}

  std::string get_identifier() const override { return "CountingResultHandler"; }

  size_t matches() const { return matches_; }
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

inline std::string read_file(const std::string& path) {
  std::ifstream in(path);
  REQUIRE(in.is_open());
  std::ostringstream buf;
  buf << in.rdbuf();
  return buf.str();
}

// smart_homes_data.csv's fixed format:
// event_type,id,plug_timestamp,value,plug_id,household_id.
inline std::vector<std::shared_ptr<Types::Event>>
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
inline std::vector<std::shared_ptr<Types::Event>>
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

inline void write_csv(const std::string& path, const std::vector<Row>& rows) {
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

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
