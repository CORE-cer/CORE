// Randomized differential test: random predicates and random events, with the
// baseline evaluator as the ground truth.
//
// The hand-written tests only cover the cases someone thought of. Here a seeded
// generator builds many predicate sets out of every atom family the translator
// handles (comparisons, attribute vs attribute, linear arithmetic, IN RANGE,
// mixed int/double), the atoms it must leave opaque (regex, string equality,
// double arithmetic, nonlinear products, `%` and `/` by a literal), and
// And/Or/Not around them, for one event type, the other, or both. Each set is then
// evaluated by the baseline and by the minterm tree on random events biased
// towards the values where evaluation is delicate (thresholds and their
// neighbours, zero and negative zero, NaN, infinities, 2^53).
//
// The seeds are fixed, so a failure always reproduces: the failing seed and the
// predicates are printed. Re-run just this test and read the seed from the log.
//
// Values are kept clear of int64_t overflow (signed overflow is undefined
// behaviour in the baseline itself): "narrow" seeds use small ints and may
// contain arithmetic, "wide" seeds use extreme ints (INT64_MIN/MAX, 2^53 and
// its neighbours) but no arithmetic on them.

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/and_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_math_exprs.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_constant.hpp"
#include "core_server/internal/evaluation/physical_predicate/comparison_type.hpp"
#include "core_server/internal/evaluation/physical_predicate/in_range_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/like_predicate/compare_with_regex_strongly_typed.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/addition.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/division.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/modulo.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/multiplication.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/subtraction.hpp"
#include "core_server/internal/evaluation/physical_predicate/not_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/or_predicate.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "shared/datatypes/eventWrapper.hpp"
#include "test_support.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/predicate_builders.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests {

namespace {

// Seeds run per test. The default keeps the suite quick; to hunt for bugs set the
// environment variable CORE_MINTERM_FUZZ_SEEDS to a larger number.
constexpr uint64_t kDefaultSeeds = 100;
constexpr int kEventsPerSeed = 150;
// Keeps the build cost of each seed bounded (the tree is built with Z3, which is
// slow in a Debug build); a seed that needs more leaves just exercises the
// direct-evaluation fallback instead.
constexpr size_t kLeafCap = 48;

// Where the attributes of an event type live (see test_support.hpp).
struct Layout {
  std::vector<size_t> ints;
  std::vector<size_t> doubles;
  std::optional<size_t> string;
};

const Layout kEventType0{{kInteger1, kInteger2}, {kDouble1, kDouble2}, kString};
const Layout kEventType1{{0, 1}, {}, std::nullopt};
// Attribute 1 is an int in BOTH event types: the only one an atom that admits
// both types can read.
const Layout kBothTypes{{1}, {}, std::nullopt};

template <typename Make>
Atom with_comparison(Comparison comparison, Make&& make) {
  switch (comparison) {
    case Comparison::EQUALS:
      return make(std::integral_constant<Comparison, Comparison::EQUALS>{});
    case Comparison::GREATER:
      return make(std::integral_constant<Comparison, Comparison::GREATER>{});
    case Comparison::GREATER_EQUALS:
      return make(std::integral_constant<Comparison, Comparison::GREATER_EQUALS>{});
    case Comparison::LESS_EQUALS:
      return make(std::integral_constant<Comparison, Comparison::LESS_EQUALS>{});
    case Comparison::LESS:
      return make(std::integral_constant<Comparison, Comparison::LESS>{});
    case Comparison::NOT_EQUALS:
      break;
  }
  return make(std::integral_constant<Comparison, Comparison::NOT_EQUALS>{});
}

class RandomWorkload {
 public:
  RandomWorkload(uint64_t seed, bool wide) : rng_(seed), wide_(wide) {
    if (wide_) {
      int_pool_ = {kInt64Min,
                   -k2p53 - 1,
                   -k2p53,
                   -2,
                   -1,
                   0,
                   1,
                   2,
                   5,
                   k2p53 - 1,
                   k2p53,
                   k2p53 + 1,
                   kInt64Max};
    } else {
      for (int64_t v = -12; v <= 12; v++) int_pool_.push_back(v);
    }
    double_pool_ = {-kInf,
                    -kDoubleMax,
                    -2.5,
                    -1.0,
                    -0.0,
                    0.0,
                    5e-324,
                    0.1,
                    0.3,
                    0.5,
                    1.0,
                    2.5,
                    3.0,
                    1e16,
                    9007199254740992.0,
                    9007199254740994.0,
                    std::nextafter(0.5, 1.0),
                    std::nextafter(0.5, 0.0),
                    kDoubleMax,
                    kInf,
                    kNaN};
    strings_ = {"", "a", "ab", "abc", "b", "ba"};
  }

  std::vector<Atom> predicates() {
    std::vector<Atom> out;
    int count = uniform(2, 5);
    for (int i = 0; i < count; i++) out.push_back(random_predicate());
    return out;
  }

  Types::EventWrapper event() {
    // Draws are made one by one: function arguments have no defined order.
    if (chance(60)) {
      std::string text = pick(strings_);
      int64_t integer1 = pick(int_pool_);
      int64_t integer2 = pick(int_pool_);
      double double1 = pick(double_pool_);
      double double2 = pick(double_pool_);
      return make_event_type_1(text, integer1, integer2, double1, double2);
    }
    int64_t integer1 = pick(int_pool_);
    int64_t integer2 = pick(int_pool_);
    return make_event_type_2(integer1, integer2);
  }

 private:
  std::mt19937_64 rng_;
  bool wide_;
  std::vector<int64_t> int_pool_;
  std::vector<double> double_pool_;
  std::vector<std::string> strings_;

  int uniform(int low, int high) {
    return std::uniform_int_distribution<int>(low, high)(rng_);
  }

  bool chance(int percent) { return uniform(0, 99) < percent; }

  template <typename T>
  const T& pick(const std::vector<T>& pool) {
    return pool[static_cast<size_t>(uniform(0, static_cast<int>(pool.size()) - 1))];
  }

  size_t pick_position(const std::vector<size_t>& positions) { return pick(positions); }

  Comparison random_comparison() {
    static const std::vector<Comparison> all = {Comparison::EQUALS,
                                                Comparison::GREATER,
                                                Comparison::GREATER_EQUALS,
                                                Comparison::LESS_EQUALS,
                                                Comparison::LESS,
                                                Comparison::NOT_EQUALS};
    return pick(all);
  }

  int64_t int_literal_value() { return pick(int_pool_); }

  double double_literal_value() {
    // Non-finite literals are rare: they make the atom opaque.
    double value = pick(double_pool_);
    if (!std::isfinite(value) && !chance(20)) return 0.5;
    return value;
  }

  // ---- expressions -----------------------------------------------------------

  // Small linear int expression: attribute/literal leaves, + and -, and * by a
  // literal. Only used in narrow seeds (values small, so nothing overflows).
  Expr<int64_t> int_expr(const Layout& layout, int depth) {
    if (depth == 0 || chance(40)) {
      if (chance(70)) return attribute<int64_t>(pick_position(layout.ints));
      return literal<int64_t>(uniform(-4, 6));
    }
    switch (uniform(0, 2)) {
      case 0: {
        Expr<int64_t> left = int_expr(layout, depth - 1);
        Expr<int64_t> right = int_expr(layout, depth - 1);
        return node<CEA::Addition, int64_t>(std::move(left), std::move(right));
      }
      case 1: {
        Expr<int64_t> left = int_expr(layout, depth - 1);
        Expr<int64_t> right = int_expr(layout, depth - 1);
        return node<CEA::Subtraction, int64_t>(std::move(left), std::move(right));
      }
      default: {
        Expr<int64_t> factor = literal<int64_t>(uniform(-3, 4));
        Expr<int64_t> other = int_expr(layout, depth - 1);
        return node<CEA::Multiplication, int64_t>(std::move(factor), std::move(other));
      }
    }
  }

  Expr<int64_t> int_leaf(const Layout& layout) {
    if (chance(60)) return attribute<int64_t>(pick_position(layout.ints));
    return literal<int64_t>(int_literal_value());
  }

  Expr<double> double_leaf(const Layout& layout) {
    if (chance(60)) return attribute<double>(pick_position(layout.doubles));
    return literal<double>(double_literal_value());
  }

  // ---- atoms -----------------------------------------------------------------

  Atom random_atom(const std::set<uint64_t>& types, const Layout& layout) {
    while (true) {
      switch (uniform(0, 12)) {
        case 0:  // int attribute vs literal
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            size_t position = pick_position(layout.ints);
            int64_t threshold = int_literal_value();
            return std::make_unique<
              CEA::CompareWithConstant<decltype(comparison)::value, int64_t>>(types,
                                                                              position,
                                                                              threshold);
          });
        case 1:  // double attribute vs literal
          if (layout.doubles.empty()) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            size_t position = pick_position(layout.doubles);
            double threshold = double_literal_value();
            return std::make_unique<
              CEA::CompareWithConstant<decltype(comparison)::value, double>>(types,
                                                                             position,
                                                                             threshold);
          });
        case 2:  // int attribute vs int attribute
          if (layout.ints.size() < 2) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            return std::make_unique<
              CEA::CompareWithAttribute<decltype(comparison)::value, int64_t, int64_t>>(
              types, layout.ints[0], layout.ints[1]);
          });
        case 3:  // double attribute vs double attribute
          if (layout.doubles.size() < 2) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            return std::make_unique<
              CEA::CompareWithAttribute<decltype(comparison)::value, double, double>>(
              types, layout.doubles[0], layout.doubles[1]);
          });
        case 4:  // int vs double attribute, either order (int converts to double)
          if (layout.doubles.empty()) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            constexpr Comparison C = decltype(comparison)::value;
            bool int_first = chance(50);
            size_t int_position = pick_position(layout.ints);
            size_t double_position = pick_position(layout.doubles);
            if (int_first) {
              return std::make_unique<CEA::CompareWithAttribute<C, int64_t, double>>(
                types, int_position, double_position);
            }
            return std::make_unique<CEA::CompareWithAttribute<C, double, int64_t>>(
              types, double_position, int_position);
          });
        case 5:  // linear int arithmetic (narrow seeds only)
          if (wide_) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            Expr<int64_t> left = int_expr(layout, 2);
            Expr<int64_t> right = int_expr(layout, 1);
            return std::make_unique<
              CEA::CompareMathExprs<decltype(comparison)::value, int64_t>>(types,
                                                                           std::move(left),
                                                                           std::move(
                                                                             right));
          });
        case 6:  // double attribute/literal math expression
          if (layout.doubles.empty()) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            Expr<double> left = double_leaf(layout);
            Expr<double> right = double_leaf(layout);
            return std::make_unique<
              CEA::CompareMathExprs<decltype(comparison)::value, double>>(types,
                                                                          std::move(left),
                                                                          std::move(right));
          });
        case 7:  // an int attribute used inside a double expression
          if (layout.doubles.empty()) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            Expr<double> left = attribute<double, int64_t>(pick_position(layout.ints));
            Expr<double> right = double_leaf(layout);
            return std::make_unique<
              CEA::CompareMathExprs<decltype(comparison)::value, double>>(types,
                                                                          std::move(left),
                                                                          std::move(right));
          });
        case 8: {  // IN RANGE over ints
          Expr<int64_t> value = int_leaf(layout);
          Expr<int64_t> lower = int_leaf(layout);
          Expr<int64_t> upper = int_leaf(layout);
          return std::make_unique<CEA::InRangePredicate<int64_t>>(types,
                                                                  std::move(value),
                                                                  std::move(lower),
                                                                  std::move(upper));
        }
        case 9:  // IN RANGE over doubles
          if (layout.doubles.empty()) continue;
          {
            Expr<double> value = double_leaf(layout);
            Expr<double> lower = double_leaf(layout);
            Expr<double> upper = double_leaf(layout);
            return std::make_unique<CEA::InRangePredicate<double>>(types,
                                                                   std::move(value),
                                                                   std::move(lower),
                                                                   std::move(upper));
          }
        case 10:  // opaque: regex or string equality
          if (!layout.string.has_value() || types.size() != 1) continue;
          if (chance(50)) {
            return std::make_unique<CEA::CompareWithRegexStronglyTyped>(
              *types.begin(), *layout.string, std::string(chance(50) ? "^a" : "b$"));
          }
          return string_equals(types, chance(50) ? "a" : "ab");
        case 11:  // opaque: arithmetic on doubles (IEEE rounding differs from reals)
          if (layout.doubles.empty()) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            size_t position = pick_position(layout.doubles);
            double compared_with = pick(double_pool_);
            return std::make_unique<
              CEA::CompareMathExprs<decltype(comparison)::value, double>>(
              types,
              node<CEA::Addition, double>(attribute<double>(position),
                                          literal<double>(0.1)),
              literal<double>(compared_with));
          });
        default:  // opaque but evaluable: nonlinear product, or % and / by a literal
          if (wide_) continue;
          return with_comparison(random_comparison(), [&](auto comparison) -> Atom {
            constexpr Comparison C = decltype(comparison)::value;
            int shape = uniform(0, 2);
            size_t position = pick_position(layout.ints);
            size_t other_position = pick_position(layout.ints);
            int divisor = uniform(2, 7);
            int64_t compared_with = uniform(-2, 9);
            switch (shape) {
              case 0:
                return std::make_unique<CEA::CompareMathExprs<C, int64_t>>(
                  types,
                  node<CEA::Multiplication, int64_t>(attribute<int64_t>(position),
                                                     attribute<int64_t>(other_position)),
                  literal<int64_t>(compared_with));
              case 1:
                return std::make_unique<CEA::CompareMathExprs<C, int64_t>>(
                  types,
                  node<CEA::Modulo, int64_t>(attribute<int64_t>(position),
                                             literal<int64_t>(divisor)),
                  literal<int64_t>(compared_with));
              default:
                return std::make_unique<CEA::CompareMathExprs<C, int64_t>>(
                  types,
                  node<CEA::Division, int64_t>(attribute<int64_t>(position),
                                               literal<int64_t>(divisor)),
                  literal<int64_t>(compared_with));
            }
          });
      }
    }
  }

  // ---- predicates ------------------------------------------------------------

  // An And/Or/Not tree in which every node admits the same event types (as the
  // CEQL visitors build them).
  Atom random_tree(const std::set<uint64_t>& types, const Layout& layout, int depth) {
    if (depth == 0 || chance(45)) return random_atom(types, layout);
    auto children = [&](int count) {
      std::vector<Atom> out;
      for (int i = 0; i < count; i++)
        out.push_back(random_tree(types, layout, depth - 1));
      return out;
    };
    switch (uniform(0, 2)) {
      case 0:
        return std::make_unique<CEA::AndPredicate>(types, children(uniform(2, 3)));
      case 1:
        return std::make_unique<CEA::OrPredicate>(types, children(uniform(2, 3)));
      default:
        return std::make_unique<CEA::NotPredicate>(types,
                                                   random_tree(types, layout, depth - 1));
    }
  }

  Atom random_predicate() {
    switch (uniform(0, 8)) {
      case 0:
      case 1:
      case 2:
      case 3:
        return random_tree({0}, kEventType0, 2);
      case 4:
      case 5:
        return random_tree({1}, kEventType1, 2);
      case 6:
        return random_tree({0, 1}, kBothTypes, 2);
      default: {
        // An Or across both event types: each child only admits one of them, so
        // the Or's per-child gating decides which side runs for an event.
        std::vector<Atom> sides;
        sides.push_back(random_tree({0}, kEventType0, 1));
        sides.push_back(random_tree({1}, kEventType1, 1));
        return std::make_unique<CEA::OrPredicate>(std::set<uint64_t>{0, 1},
                                                  std::move(sides));
      }
    }
  }
};

}  // namespace

static uint64_t seeds_to_run() {
  // NOLINTNEXTLINE(concurrency-mt-unsafe): read once, before any thread starts
  const char* requested = std::getenv("CORE_MINTERM_FUZZ_SEEDS");
  if (requested == nullptr) return kDefaultSeeds;
  return std::strtoull(requested, nullptr, 10);
}

TEST_CASE("Random predicates and events give the baseline's bitsets",
          "[MintermTreeEvaluator][Optimizations][Fuzz]") {
  const uint64_t kSeeds = seeds_to_run();
  size_t seeds_with_a_split_tree = 0;
  size_t seeds_that_fell_back = 0;

  for (uint64_t seed = 0; seed < kSeeds; seed++) {
    bool wide = seed % 4 == 3;
    RandomWorkload workload(seed, wide);
    ParityChecker parity(workload.predicates(), kLeafCap);

    INFO("seed " << seed << (wide ? " (wide ints)" : " (narrow ints)"));
    INFO("predicates:\n" << parity.baseline.to_string());

    for (int i = 0; i < kEventsPerSeed; i++) {
      auto event = workload.event();
      parity.check(event, "random event #" + std::to_string(i));
    }

    if (parity.minterm.debug_leaf_count(0) > 2 || parity.minterm.debug_leaf_count(1) > 2) {
      seeds_with_a_split_tree++;
    }
    if (parity.minterm.uses_direct_evaluation(0)
        || parity.minterm.uses_direct_evaluation(1)) {
      seeds_that_fell_back++;
    }
  }

  // The generator must exercise BOTH paths, or the test proves little: most
  // seeds build a tree that splits more than once, and some go over the leaf
  // cap and use the direct-evaluation fallback (which must also match).
  INFO("seeds with a tree of more than two leaves: "
       << seeds_with_a_split_tree
       << ", seeds with a direct-evaluation fallback: " << seeds_that_fell_back);
  if (kSeeds >= 50) {  // too few seeds say nothing about the mix
    REQUIRE(seeds_with_a_split_tree >= kSeeds / 2);
    REQUIRE(seeds_that_fell_back > 0);
    REQUIRE(seeds_that_fell_back < kSeeds);
  }
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests
