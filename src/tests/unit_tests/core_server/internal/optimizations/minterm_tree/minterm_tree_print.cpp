// Tests for MintermTreeEvaluator::trees_to_string / print_trees: the orderly
// dump of the trees an evaluator built, meant for debugging and for checking in
// unit tests that a set of predicates gives the tree one expects.
//
// Most tests compare the WHOLE text, so a change to the format is a visible,
// deliberate one. Region formulas (the `region:` text) are Z3's own printing and
// may differ between Z3 versions, so exact-text tests turn them off and only
// looser tests look at them.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_tree_evaluator.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree_printing.hpp"
#include "test_support.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

TreePrintOptions without_regions() {
  TreePrintOptions options;
  options.show_regions = false;
  return options;
}

bool contains(const std::string& text, const std::string& part) {
  return text.find(part) != std::string::npos;
}

std::vector<Atom> worked_example() {
  // The example from the module guide: three predicates on one attribute.
  std::vector<Atom> predicates;
  predicates.push_back(greater_at({0}, kInteger1, 100));
  predicates.push_back(greater_at({0}, kInteger1, 50));
  predicates.push_back(less_at({0}, kInteger1, 10));
  return predicates;
}

}  // namespace

TEST_CASE("The trees of the worked example print as expected",
          "[MintermTreeEvaluator][Optimizations][Print]") {
  ParityChecker parity(worked_example());

  // x > 100 implies x > 50 and excludes x < 10, so four leaves, not eight.
  const std::string expected =
    "MintermTreeEvaluator: 3 predicates (0 evaluated directly), 3 atoms (0 opaque), "
    "1 trees with 4 leaves in total (largest 4)\n"
    "\n"
    "Predicates\n"
    "  [0] Event[1] > 100   (tree)\n"
    "  [1] Event[1] > 50   (tree)\n"
    "  [2] Event[1] < 10   (tree)\n"
    "\n"
    "Event type 0: 4 leaves, 3 atoms\n"
    "  Event[1] > 100 ?\n"
    "  +-- true:  leaf #0 -> p0 p1\n"
    "  `-- false: Event[1] > 50 ?\n"
    "      +-- true:  leaf #1 -> p1\n"
    "      `-- false: Event[1] < 10 ?\n"
    "          +-- true:  leaf #2 -> p2\n"
    "          `-- false: leaf #3 -> (none)\n";
  REQUIRE(parity.minterm.trees_to_string(without_regions()) == expected);
}

TEST_CASE("Regions are shown by default and can be cut short",
          "[MintermTreeEvaluator][Optimizations][Print]") {
  ParityChecker parity(worked_example());

  SECTION("by default every leaf carries its region") {
    std::string text = parity.minterm.trees_to_string();
    size_t leaves_with_region = 0;
    std::istringstream lines(text);
    for (std::string line; std::getline(lines, line);) {
      if (contains(line, "leaf #")) {
        REQUIRE(contains(line, "   region: "));
        leaves_with_region++;
      }
    }
    REQUIRE(leaves_with_region == 4);
  }

  SECTION("a region longer than the limit ends in ...") {
    TreePrintOptions options;
    options.max_region_chars = 8;
    std::istringstream lines(parity.minterm.trees_to_string(options));
    size_t checked = 0;
    for (std::string line; std::getline(lines, line);) {
      size_t at = line.find("   region: ");
      if (at == std::string::npos) continue;
      std::string region = line.substr(at + std::string("   region: ").size());
      // Either it fits, or it is cut to the limit plus the marker.
      REQUIRE(region.size() <= 8 + 3);
      if (region.size() > 8) REQUIRE(region.ends_with("..."));
      checked++;
    }
    REQUIRE(checked == 4);
  }
}

TEST_CASE("Opaque atoms are tagged", "[MintermTreeEvaluator][Optimizations][Print]") {
  std::vector<Atom> predicates;
  predicates.push_back(greater_at({0}, kInteger1, 5));
  predicates.push_back(
    std::make_unique<CEA::CompareWithRegexStronglyTyped>(0, kString, std::string("^a")));
  ParityChecker parity(std::move(predicates));

  std::string text = parity.minterm.trees_to_string(without_regions());
  // The regex is opaque, the comparison is not.
  REQUIRE(contains(text, "[opaque] ?"));
  REQUIRE(contains(text, "Event[1] > 5 ?"));
  REQUIRE(!contains(text, "Event[1] > 5 [opaque]"));
  REQUIRE(contains(text, "2 atoms (1 opaque)"));
}

TEST_CASE("Predicates evaluated directly say why",
          "[MintermTreeEvaluator][Optimizations][Print]") {
  std::vector<Atom> predicates;
  // 0: divides by an attribute, so it could fail if evaluated early.
  {
    std::vector<Atom> children;
    children.push_back(
      std::make_unique<CEA::CompareWithConstant<Comparison::NOT_EQUALS, int64_t>>(
        0, kInteger2, int64_t{0}));
    children.push_back(
      std::make_unique<CEA::CompareMathExprs<Comparison::GREATER, int64_t>>(
        0,
        node<CEA::Division, int64_t>(attribute<int64_t>(kInteger1),
                                     attribute<int64_t>(kInteger2)),
        literal<int64_t>(1)));
    predicates.push_back(std::make_unique<CEA::AndPredicate>(0, std::move(children)));
  }
  // 1: admits any event type.
  {
    Atom any_type = greater_at({0}, kInteger1, 3);
    any_type->admissible_event_types.clear();
    any_type->admits_any_event_type = true;
    predicates.push_back(std::move(any_type));
  }
  // 2: an ordinary predicate, which gets a tree.
  predicates.push_back(greater_at({0}, kInteger1, 5));
  ParityChecker parity(std::move(predicates));

  std::string text = parity.minterm.trees_to_string(without_regions());
  REQUIRE(contains(text, "(direct: an atom could fail if evaluated early"));
  REQUIRE(contains(text, "(direct: it admits any event type)"));
  REQUIRE(contains(text, "[2] Event[1] > 5   (tree)"));
  REQUIRE(contains(text, "(2 evaluated directly)"));
}

TEST_CASE("Event types are listed in order, those without a tree with their reason",
          "[MintermTreeEvaluator][Optimizations][Print]") {
  // Event type 0 has five independent regexes (32 leaves): with a cap of 8 it is
  // evaluated directly. Event type 1 has one comparison and keeps its tree.
  std::vector<Atom> predicates;
  for (const char* pattern : {"^a", "^b", "^c", "^d", "^e"}) {
    predicates.push_back(
      std::make_unique<CEA::CompareWithRegexStronglyTyped>(0,
                                                           kString,
                                                           std::string(pattern)));
  }
  predicates.push_back(greater_at({1}, 0, 5));
  ParityChecker parity(std::move(predicates), 8);

  std::string text = parity.minterm.trees_to_string(without_regions());
  size_t type_0 = text.find(
    "Event type 0: evaluated directly (minterm tree exceeded 8 leaves)");
  size_t type_1 = text.find("Event type 1: 2 leaves, 1 atoms");
  REQUIRE(type_0 != std::string::npos);
  REQUIRE(type_1 != std::string::npos);
  REQUIRE(type_0 < type_1);
}

TEST_CASE("Printing does not change the evaluator",
          "[MintermTreeEvaluator][Optimizations][Print]") {
  ParityChecker parity(worked_example());
  std::string summary_before = parity.minterm.describe();

  // Printing twice gives the same text, into a stream or a string.
  std::string first = parity.minterm.trees_to_string();
  std::ostringstream streamed;
  parity.minterm.print_trees(streamed);
  REQUIRE(streamed.str() == first);
  REQUIRE(parity.minterm.trees_to_string() == first);

  // describe() and the results are untouched.
  REQUIRE(parity.minterm.describe() == summary_before);
  for (int64_t integer1 : {int64_t{-5}, int64_t{10}, int64_t{60}, int64_t{101}}) {
    auto event = make_event_type_1("s", integer1, 0, 0.0, 0.0);
    parity.check(event, "Integer1 = " + std::to_string(integer1));
  }
}

TEST_CASE("An evaluator without predicates prints an empty listing",
          "[MintermTreeEvaluator][Optimizations][Print]") {
  ParityChecker parity(std::vector<Atom>{});
  REQUIRE(parity.minterm.trees_to_string(without_regions())
          == "MintermTreeEvaluator: 0 predicates (0 evaluated directly), 0 atoms (0 opaque), "
             "0 trees with 0 leaves in total (largest 0)\n"
             "\n"
             "Predicates\n");
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
