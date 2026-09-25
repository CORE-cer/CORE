// Tests for printing a minterm tree (MintermTreeNode::print) and for the
// environment switch that asks for trees to be printed. The printer is generic
// (it only needs callbacks that say what atoms, regions and leaves look like), so
// it is tested here on the toy WorldAlgebra without Z3; the evaluator-level
// output, with real predicates, is tested in minterm_tree/minterm_tree_print.cpp.

#include <bit>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <memory>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "core_server/internal/optimizations/minterm_tree/minterm_generator.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree.hpp"
#include "core_server/internal/optimizations/minterm_tree/tree_printing.hpp"
#include "toy_algebra.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

namespace {

using Node = MintermTreeNode<uint64_t>;

// Distinct atom objects; the printer only shows what the label callback says.
class Atoms {
 public:
  std::vector<std::unique_ptr<CEA::PhysicalPredicate>> owned;
  std::vector<CEA::PhysicalPredicate*> pointers;
  std::map<const CEA::PhysicalPredicate*, std::string> names;

  explicit Atoms(size_t count) {
    for (size_t i = 0; i < count; i++) {
      owned.push_back(
        std::make_unique<CEA::CompareWithConstant<CEA::ComparisonType::GREATER, int64_t>>(
          0, 0, static_cast<int64_t>(i)));
      pointers.push_back(owned.back().get());
      names[pointers.back()] = "a" + std::to_string(i);
    }
  }
};

std::string hex(uint64_t value) {
  std::ostringstream out;
  out << std::hex << std::setw(16) << std::setfill('0') << value;
  return out.str();
}

Node::Printer printer_for(const Atoms& atoms, bool with_regions = true) {
  Node::Printer printer;
  printer.atom_label = [&atoms](const CEA::PhysicalPredicate* atom) {
    return atoms.names.at(atom);
  };
  printer.leaf_label = [](const Node& leaf) {
    return "size " + std::to_string(std::popcount(leaf.phi));
  };
  if (with_regions)
    printer.region_label = [](const uint64_t& region) { return hex(region); };
  return printer;
}

std::string
printed(const Node& tree, const Node::Printer& printer, const std::string& indent = "") {
  std::ostringstream out;
  tree.print(out, printer, indent);
  return out.str();
}

std::unique_ptr<Node> tree_of(const Atoms& atoms, const std::vector<uint64_t>& formulas) {
  WorldAlgebra algebra;
  return buildMintermTree<uint64_t>(atoms.pointers, formulas, algebra, size_t{1} << 20);
}

}  // namespace

TEST_CASE("A tree prints depth-first with the true branch first",
          "[MintermTreeCore][Optimizations][Print]") {
  Atoms atoms(2);
  auto tree = tree_of(atoms, WorldAlgebra::independent_variables(2));

  // The two independent variables split the 64 worlds into four regions of 16.
  const std::string expected =
    "  a0 ?\n"
    "  +-- true:  a1 ?\n"
    "  |   +-- true:  leaf #0 -> size 16   region: 8888888888888888\n"
    "  |   `-- false: leaf #1 -> size 16   region: 2222222222222222\n"
    "  `-- false: a1 ?\n"
    "      +-- true:  leaf #2 -> size 16   region: 4444444444444444\n"
    "      `-- false: leaf #3 -> size 16   region: 1111111111111111\n";
  REQUIRE(printed(*tree, printer_for(atoms), "  ") == expected);
}

TEST_CASE("The region text is optional, and an empty region adds nothing",
          "[MintermTreeCore][Optimizations][Print]") {
  Atoms atoms(1);
  auto tree = tree_of(atoms, WorldAlgebra::independent_variables(1));

  REQUIRE(printed(*tree, printer_for(atoms, /*with_regions=*/false))
          == "a0 ?\n"
             "+-- true:  leaf #0 -> size 32\n"
             "`-- false: leaf #1 -> size 32\n");

  Node::Printer blank_regions = printer_for(atoms);
  blank_regions.region_label = [](const uint64_t&) { return std::string(); };
  REQUIRE(printed(*tree, blank_regions)
          == "a0 ?\n"
             "+-- true:  leaf #0 -> size 32\n"
             "`-- false: leaf #1 -> size 32\n");
}

TEST_CASE("A tree that never split prints just its single leaf",
          "[MintermTreeCore][Optimizations][Print]") {
  Atoms atoms(1);
  WorldAlgebra algebra;
  // An atom that is always true never splits the whole-space region.
  auto tree = tree_of(atoms, {algebra.truePredicate()});
  REQUIRE(printed(*tree, printer_for(atoms), "> ")
          == "> leaf #0 -> size 64   region: ffffffffffffffff\n");
}

TEST_CASE("Leaf numbers follow collectLeaves and every node is printed once",
          "[MintermTreeCore][Optimizations][Print]") {
  // Random formulas give trees of many shapes. The numbering must match the order
  // collectLeaves returns (so the two can be cross-referenced), and a full binary
  // tree with L leaves has L - 1 conditions.
  for (uint64_t seed = 0; seed < 20; seed++) {
    std::mt19937_64 rng(seed);
    std::vector<uint64_t> formulas;
    for (int i = 0; i < 4; i++) formulas.push_back(rng());
    Atoms atoms(formulas.size());
    auto tree = tree_of(atoms, formulas);
    INFO("seed " << seed);

    std::vector<const Node*> leaves;
    tree->collectLeaves(leaves);
    const std::string text = printed(*tree, printer_for(atoms));

    std::regex leaf_line(R"(leaf #(\d+) -> size \d+   region: ([0-9a-f]{16}))");
    size_t seen = 0;
    for (std::sregex_iterator it(text.begin(), text.end(), leaf_line), end; it != end;
         ++it) {
      size_t number = std::stoull((*it)[1]);
      REQUIRE(number == seen);
      REQUIRE(number < leaves.size());
      REQUIRE((*it)[2] == hex(leaves[number]->phi));
      seen++;
    }
    REQUIRE(seen == leaves.size());

    size_t conditions = 0;
    for (size_t at = text.find(" ?\n"); at != std::string::npos;
         at = text.find(" ?\n", at + 1)) {
      conditions++;
    }
    REQUIRE(conditions == leaves.size() - 1);
  }
}

TEST_CASE("Every printed line starts with the indent",
          "[MintermTreeCore][Optimizations][Print]") {
  Atoms atoms(3);
  auto tree = tree_of(atoms, WorldAlgebra::independent_variables(3));
  std::istringstream lines(printed(*tree, printer_for(atoms), ">>"));
  std::string line;
  size_t count = 0;
  while (std::getline(lines, line)) {
    REQUIRE(line.starts_with(">>"));
    count++;
  }
  REQUIRE(count == 7 + 8);  // 7 conditions and 8 leaves
}

TEST_CASE("CORE_PRINT_MINTERM_TREES switches printing on unless empty or 0",
          "[MintermTreeCore][Optimizations][Print]") {
  // NOLINTBEGIN(concurrency-mt-unsafe): single-threaded test, environment restored below
  const char* previous = std::getenv("CORE_PRINT_MINTERM_TREES");
  const std::string saved = previous != nullptr ? previous : "";
  const bool was_set = previous != nullptr;

  unsetenv("CORE_PRINT_MINTERM_TREES");
  REQUIRE(!print_trees_requested());
  setenv("CORE_PRINT_MINTERM_TREES", "", 1);
  REQUIRE(!print_trees_requested());
  setenv("CORE_PRINT_MINTERM_TREES", "0", 1);
  REQUIRE(!print_trees_requested());
  setenv("CORE_PRINT_MINTERM_TREES", "1", 1);
  REQUIRE(print_trees_requested());
  setenv("CORE_PRINT_MINTERM_TREES", "yes", 1);
  REQUIRE(print_trees_requested());

  if (was_set) {
    setenv("CORE_PRINT_MINTERM_TREES", saved.c_str(), 1);
  } else {
    unsetenv("CORE_PRINT_MINTERM_TREES");
  }
  // NOLINTEND(concurrency-mt-unsafe)
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
