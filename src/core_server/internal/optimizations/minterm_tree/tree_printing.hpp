#pragma once

#include <cstddef>
#include <cstdlib>
#include <string_view>

namespace CORE::Internal::Optimizations::MintermTree {

// Options for printing the trees of a MintermTreeEvaluator
// (MintermTreeEvaluator::trees_to_string / print_trees). Printing is a debugging
// aid, mostly for the unit tests: it shows the trees the evaluator actually
// built, so it is easy to check that the tree of a query looks as expected.
struct TreePrintOptions {
  // Show, next to each leaf, the formula that describes its region. Long for
  // big trees, hence the cap below.
  bool show_regions = true;
  // A region longer than this is cut and ends in "...".
  size_t max_region_chars = 100;
};

// True when the environment asks for the trees to be printed
// (CORE_PRINT_MINTERM_TREES set to anything but empty or "0"). The evaluator
// factory then logs the trees of every query it builds, and the unit-test
// helpers print the trees they build, without changing any code:
//
//   CORE_PRINT_MINTERM_TREES=1 ./build/Debug-opt/tests "[Structure]"
inline bool print_trees_requested() {
  // NOLINTNEXTLINE(concurrency-mt-unsafe): read while a query is being set up
  const char* value = std::getenv("CORE_PRINT_MINTERM_TREES");
  return value != nullptr && *value != '\0' && std::string_view(value) != "0";
}

}  // namespace CORE::Internal::Optimizations::MintermTree
