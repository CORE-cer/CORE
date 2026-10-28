// Ground-truth validation of the analytical L̄ (average tree-path-length-
// per-event) estimate in tests/benchmark/path_length_model.py.
//
// That script estimates L̄ from the tree's printed shape plus each
// attribute's measured selectivity - an expected value under a probability
// model, never an actual count of what the engine does. This test is the
// actual count: it compiles a handful of the real smart_homes queries
// (src/targets/experiments/smart_homes/queries_optimization/*.txt) against
// the real smart_homes schema, builds a MintermTreeEvaluator exactly as
// the benchmark does (AsDiscovered ordering, Opaque string equality - the
// "minterm_tree" strategy the model was fit against), and streams every
// real event from smart_homes_data.csv through
// MintermTreeEvaluator::debug_path_length (minterm_tree_evaluator.hpp),
// which mirrors operator()'s walk but counts decisions instead of
// producing a Bitset. The ground-truth mean is printed next to what the
// Python script predicts for the same query, for comparison by hand - this
// is exploratory validation, not a tight pass/fail check, since the
// model's one real assumption (cross-attribute independence) is expected
// to cost it some accuracy.
//
// Reads the real 1,000,000-row dataset, so this must not run routinely.
// Tagged hidden ([.]) for a bare `./tests` run - but Catch2's hidden-tag
// exclusion does NOT apply once any tag filter is given on the command
// line (checked directly: `./tests "[Optimizations]"` ran this test; the
// `[.]` tag alone does not save it from a positive-tag filter). Left out
// of [Optimizations] and [MintermTreeEvaluator] entirely instead, so
// `build_and_test_optimizations.sh`/`run_optimization_tests.sh` (which
// filter by "[Optimizations]") never match it either. Runs only when asked
// for explicitly, by its own tag:
//   ./build/Debug-opt/tests "[PathLengthGroundTruth]"

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_tree_evaluator.hpp"
#include "shared/datatypes/catalog/attribute_info.hpp"
#include "shared/datatypes/catalog/stream_info.hpp"
#include "shared/datatypes/event.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "shared/datatypes/value.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/ceql_fixture.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/minterm_tree/test_support.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

// The real smart_homes schema (declaration.core), reproduced here rather
// than parsed from the file - it's five lines and changing it without
// noticing here would be a bigger problem than keeping this in sync.
Types::UniqueEventTypeId declare_smart_homes(Catalog& catalog) {
  std::vector<Types::AttributeInfo> load_attrs;
  load_attrs.emplace_back("id", Types::ValueTypes::INT64);
  load_attrs.emplace_back("plug_timestamp", Types::ValueTypes::INT64);
  load_attrs.emplace_back("value", Types::ValueTypes::DOUBLE);
  load_attrs.emplace_back("plug_id", Types::ValueTypes::INT64);
  load_attrs.emplace_back("household_id", Types::ValueTypes::INT64);
  Types::StreamInfo info = catalog.add_stream_type(
    {"S", {{"LOAD", std::move(load_attrs)}}});
  return info.events_info.at(0).id;
}

// A full, already-complete CEQL query (unlike CompiledFilter in
// ceql_fixture.hpp, which wraps a bare FILTER clause into its own fixed
// query template). parse_query only extracts FROM/WHERE either way, so
// handing it the real query text as-is - WITHIN/CONSUME BY/LIMIT and all -
// gives the exact same predicates the engine built when this query was
// actually benchmarked, with no risk of the test's query text drifting
// from what was really run.
class CompiledQuery {
  Catalog catalog_;
  CEQL::Query query_;
  QueryCatalog query_catalog_;

 public:
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> predicates;

  explicit CompiledQuery(const std::string& full_query_text)
      : query_([&] {
          declare_smart_homes(catalog_);
          return parse_query(full_query_text, catalog_);
        }()),
        query_catalog_(catalog_, query_),
        predicates(get_predicates(query_, query_catalog_)) {}

  CompiledQuery(const CompiledQuery&) = delete;
  CompiledQuery& operator=(const CompiledQuery&) = delete;
};

std::string read_file(const std::string& path) {
  std::ifstream in(path);
  REQUIRE(in.is_open());
  std::ostringstream buf;
  buf << in.rdbuf();
  return buf.str();
}

// smart_homes_data.csv's fixed format: event_type,id,plug_timestamp,value,plug_id,household_id.
// No general CSV parser exists in C++ in this repo (only Python's
// tests/e2e/csv_parser.py) - this dataset's shape is simple enough not to need one.
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

double average_path_length(const std::vector<std::shared_ptr<const Types::Event>>& events,
                           MintermTreeEvaluator& evaluator) {
  size_t total = 0;
  for (const auto& event : events) {
    Types::EventWrapper wrapper(event);
    total += evaluator.debug_path_length(wrapper);
  }
  return static_cast<double>(total) / static_cast<double>(events.size());
}

}  // namespace

TEST_CASE("Ground-truth L bar matches the analytical estimate, within reason",
          "[.][PathLengthGroundTruth]") {
  const std::string root = CORE_SOURCE_ROOT;
  const std::string base = root + "/src/targets/experiments/smart_homes";
  Catalog probe_catalog;
  Types::UniqueEventTypeId event_type = declare_smart_homes(probe_catalog);

  INFO("Parsing the real 1,000,000-row dataset once for every query below");
  std::vector<std::shared_ptr<const Types::Event>>
    events = read_smart_homes_events(base + "/smart_homes_data.csv", event_type);
  REQUIRE(events.size() == 1'000'000);

  // One representative query per family: a ladder, a mix, the best case, the
  // attempted worst case, and the model-targeted late-exit family (the point
  // of this round) - see the plan for why these specific ones.
  std::vector<std::string> queries = {
    "eq_ladder_16_guard.txt",
    "mix_interleaved_16_guard.txt",
    "rep_eq_16.txt",
    "worst_mix_16.txt",
    "late_exit_thr_08.txt",
    "late_exit_thr_12.txt",
    "late_exit_thr_16.txt",
  };

  for (const std::string& name : queries) {
    std::string query_text = read_file(base + "/queries_optimization/" + name);
    CompiledQuery compiled(query_text);
    // ParityChecker builds both evaluators from the same predicates and,
    // along the way, does the unique_ptr -> shared_ptr conversion
    // MintermTreeEvaluator's constructor needs (see
    // Evaluation::PredicateEvaluator's own constructor) - reused here rather
    // than duplicated, even though the baseline evaluator it also builds
    // isn't used by this test (parity is already exhaustively checked by
    // the digest-based benchmark pipeline).
    ParityChecker checker(std::move(compiled.predicates),
                          MintermTreeEvaluator::kMaxLeavesPerTree,
                          AtomOrderingStrategy::AsDiscovered,
                          StringEqualityStrategy::Opaque);
    double ground_truth_l_bar = average_path_length(events, checker.minterm);
    std::cout << name << ": ground-truth L_bar = " << ground_truth_l_bar << "\n";

    // Sanity bound only - the real analytical-vs-ground-truth comparison is
    // done by hand against tests/benchmark/path_length_model.py's output for
    // the same query, not hardcoded here.
    REQUIRE(ground_truth_l_bar >= 0.0);
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
