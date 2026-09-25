// Tests for how weakly typed predicates describe themselves as formulas.
//
// A predicate is weakly typed when its FILTER variable is not a single event type
// (an alias such as `X` in `S>event1 as X`, then `X[Integer1 > 5]`). Its
// attributes are NonStronglyTypedAttribute nodes: they find the attribute BY
// NAME, per event, through the QueryCatalog, so the position (and even the
// stored type) can differ from one event type to another. The minterm tree is
// built per event type, so the same lookup can be made once per event type when
// the formula is built. These tests check that this is done, and that it gives
// the same formula a strongly typed predicate would.
//
// Real CEQL is compiled (CompiledFilter) and the result is written as text by a
// RecordingBuilder, e.g. `(> i(0,1) 100)` for "attribute 1 of event type 0 is
// greater than 100". No Z3 is needed, so these also run in the default build.
//
// Event types of the default fixture (see ceql_fixture.hpp):
//   0 = event1: String, Integer1 (pos 1), Integer2 (pos 2), Double1, Double2
//   1 = event2: Integer1 (pos 0), Integer2 (pos 1)

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "core_server/internal/evaluation/physical_predicate/math_expr/non_strongly_typed_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/physical_predicate.hpp"
#include "recording_builder.hpp"
#include "tests/unit_tests/core_server/internal/optimizations/ceql_fixture.hpp"

namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore {

namespace {

// The formula of a predicate for `event_type`, as text.
std::string formula_of(const CEA::PhysicalPredicate& predicate, uint64_t event_type) {
  RecordingBuilder builder;
  return builder.text(predicate.translate(builder, event_type));
}

bool is_opaque(const std::string& formula) { return formula.starts_with("opaque["); }

// The text of a translated node, or a marker when it could not be translated.
std::string text_of(RecordingBuilder& builder,
                    const std::optional<CEA::FormulaBuilder::Handle>& handle) {
  return handle.has_value() ? builder.text(*handle) : std::string("(not translatable)");
}

// A weakly typed attribute node, `GlobalType` being the type the predicate works
// with. Built on the heap, like the engine builds it.
template <typename GlobalType>
std::unique_ptr<CEA::NonStronglyTypedAttribute<GlobalType>>
weak_attribute(const char* name, const QueryCatalog& catalog) {
  return std::make_unique<CEA::NonStronglyTypedAttribute<GlobalType>>(name, catalog);
}

const std::set<uint64_t> kBothEventTypes = {0, 1};
const std::set<uint64_t> kOnlyEventType1 = {0};  // event1 is unique event type 0

// A catalog in which the same attribute name is stored differently by two
// events, and with attributes of every kind the translation does not model.
//   event1 (type 0): Value INT64, Flag BOOL, When DATE, Text STRING, Count INT64
//   event2 (type 1): Value DOUBLE, Count INT64
void declare_mixed_types(Catalog& catalog) {
  std::vector<Types::AttributeInfo> first;
  first.emplace_back("Value", Types::ValueTypes::INT64);
  first.emplace_back("Flag", Types::ValueTypes::BOOL);
  first.emplace_back("When", Types::ValueTypes::DATE);
  first.emplace_back("Text", Types::ValueTypes::STRING_VIEW);
  first.emplace_back("Count", Types::ValueTypes::INT64);
  std::vector<Types::AttributeInfo> second;
  second.emplace_back("Value", Types::ValueTypes::DOUBLE);
  second.emplace_back("Count", Types::ValueTypes::INT64);
  Types::StreamInfo stream_info = catalog.add_stream_type(
    {"S", {{"event1", std::move(first)}, {"event2", std::move(second)}}});
  (void)stream_info;  // add_stream_type is [[nodiscard]]; only the side effect matters
}

}  // namespace

// ---------------------------------------------------------------------------
// The position is resolved per event type
// ---------------------------------------------------------------------------

TEST_CASE("A weakly typed attribute is resolved per event type",
          "[MintermTreeCore][Optimizations][Weak]") {
  // Integer1 is attribute 1 of event1 (type 0) and attribute 0 of event2 (type 1).
  CompiledFilter compiled("X[Integer1 > 100]");
  auto predicates = compiled.filter_predicates();
  REQUIRE(predicates.size() == 1);

  // Admitting both types is what tells it apart from a strongly typed predicate.
  REQUIRE(predicates[0]->admissible_event_types == kBothEventTypes);
  REQUIRE(formula_of(*predicates[0], 0) == "(> i(0,1) 100)");
  REQUIRE(formula_of(*predicates[0], 1) == "(> i(1,0) 100)");
}

TEST_CASE("A weakly typed predicate only applies to event types that have the attribute",
          "[MintermTreeCore][Optimizations][Weak]") {
  CompiledFilter compiled("X[Double1 > 1.5]");  // only event1 has Double1
  auto predicates = compiled.filter_predicates();
  REQUIRE(predicates.size() == 1);
  REQUIRE(predicates[0]->admissible_event_types == kOnlyEventType1);
  REQUIRE(formula_of(*predicates[0], 0) == "(> d(0,3) 1.5)");
  // Not admitted for event type 1: the translation is `false`, like evaluation.
  REQUIRE(formula_of(*predicates[0], 1) == "false");
}

TEST_CASE("A strongly and a weakly typed predicate on one attribute share its variable",
          "[MintermTreeCore][Optimizations][Weak]") {
  // The strong one finds Integer1 by position, the weak one by name; for event
  // type 0 both must talk about the same attribute, or the solver could never
  // relate them.
  CompiledFilter compiled("event1[Integer1 > 5] AND X[Integer1 < 3]");
  auto predicates = compiled.filter_predicates();
  REQUIRE(predicates.size() == 2);
  REQUIRE(predicates[0]->admissible_event_types == kOnlyEventType1);  // strong
  REQUIRE(predicates[1]->admissible_event_types == kBothEventTypes);  // weak
  REQUIRE(formula_of(*predicates[0], 0) == "(> i(0,1) 5)");
  REQUIRE(formula_of(*predicates[1], 0) == "(< i(0,1) 3)");
}

// ---------------------------------------------------------------------------
// Everything that can be built from weak attributes
// ---------------------------------------------------------------------------

TEST_CASE("Weakly typed arithmetic, ranges and combinations are translated",
          "[MintermTreeCore][Optimizations][Weak]") {
  struct Case {
    const char* filter;
    const char* type_0;
    const char* type_1;
  };

  const std::vector<Case> cases = {
    {"X[Integer1 + Integer2 > 5]", "(> (+ i(0,1) i(0,2)) 5)", "(> (+ i(1,0) i(1,1)) 5)"},
    {"X[Integer1 - 2 <= Integer2]",
     "(<= (- i(0,1) 2) i(0,2))",
     "(<= (- i(1,0) 2) i(1,1))"},
    {"X[Integer1 * 3 >= Integer2]",
     "(>= (* i(0,1) 3) i(0,2))",
     "(>= (* i(1,0) 3) i(1,1))"},
    {"X[Integer1 IN RANGE (1, 9)]",
     "(and (>= i(0,1) 1) (<= i(0,1) 9))",
     "(and (>= i(1,0) 1) (<= i(1,0) 9))"},
    // CEQL pushes a NOT into the comparison when it parses, so no Not node remains.
    {"X[NOT Integer1 > 100]", "(<= i(0,1) 100)", "(<= i(1,0) 100)"},
    {"X[Integer1 > 5 OR Integer2 < 2]",
     "(or (> i(0,1) 5) (< i(0,2) 2))",
     "(or (> i(1,0) 5) (< i(1,1) 2))"},
    {"X[Integer1 > 5 AND Integer2 < 2]",
     "(and (> i(0,1) 5) (< i(0,2) 2))",
     "(and (> i(1,0) 5) (< i(1,1) 2))"},
    {"X[Integer1 = Integer2]", "(== i(0,1) i(0,2))", "(== i(1,0) i(1,1))"},
  };
  for (const Case& test_case : cases) {
    INFO("filter: " << test_case.filter);
    CompiledFilter compiled(test_case.filter);
    auto predicates = compiled.filter_predicates();
    REQUIRE(predicates.size() == 1);
    REQUIRE(formula_of(*predicates[0], 0) == test_case.type_0);
    REQUIRE(formula_of(*predicates[0], 1) == test_case.type_1);
  }
}

TEST_CASE("Weakly typed int and double attributes mix like strongly typed ones",
          "[MintermTreeCore][Optimizations][Weak]") {
  // Comparing an int with a double converts the int to double (C++ rounding),
  // exactly as for a strongly typed predicate; both attributes exist in event1.
  CompiledFilter compiled("X[Integer1 > Double1] AND X[Double1 IN RANGE (0.5, 2.5)]");
  auto predicates = compiled.filter_predicates();
  REQUIRE(predicates.size() == 2);
  REQUIRE(formula_of(*predicates[0], 0) == "(> toDouble(i(0,1)) d(0,3))");
  REQUIRE(formula_of(*predicates[1], 0) == "(and (>= d(0,3) 0.5) (<= d(0,3) 2.5))");
}

TEST_CASE("What is not modeled stays opaque for weakly typed predicates too",
          "[MintermTreeCore][Optimizations][Weak]") {
  for (const char* filter : {
         "X[Integer1 / 2 > 3]",         // Z3's division differs from C++'s
         "X[Integer1 % 3 = 1]",         // same for modulo
         "X[Integer1 * Integer2 > 3]",  // nonlinear
         "X[Double1 + Double2 > 3.5]",  // IEEE rounding differs from exact reals
         "X[String = 'abc']",           // strings are not modeled
         "X[String LIKE 'a.*']",        // regular expressions neither
       }) {
    INFO("filter: " << filter);
    CompiledFilter compiled(filter);
    auto predicates = compiled.filter_predicates();
    REQUIRE(predicates.size() == 1);
    REQUIRE(is_opaque(formula_of(*predicates[0], 0)));
  }
}

// ---------------------------------------------------------------------------
// The attribute node itself
// ---------------------------------------------------------------------------

TEST_CASE("The weak attribute node resolves name and type from the catalog",
          "[MintermTreeCore][Optimizations][Weak]") {
  CompiledFilter compiled("X[Integer1 > 0]");
  const QueryCatalog& catalog = compiled.query_catalog();
  RecordingBuilder builder;
  auto text = [&](const auto& node, uint64_t event_type) {
    auto handle = node->translate(builder, event_type);
    return handle.has_value() ? builder.text(*handle) : std::string("(not translatable)");
  };

  // The predicate works with int64 and the event stores int64: an integer.
  auto integer = weak_attribute<int64_t>("Integer1", catalog);
  REQUIRE(text(integer, 0) == "i(0,1)");
  REQUIRE(text(integer, 1) == "i(1,0)");

  // The predicate works with double: an int64 attribute is converted, a double is
  // used as is.
  auto integer_as_double = weak_attribute<double>("Integer1", catalog);
  REQUIRE(text(integer_as_double, 0) == "toDouble(i(0,1))");
  auto real = weak_attribute<double>("Double1", catalog);
  REQUIRE(text(real, 0) == "d(0,3)");

  // A double read where an int is expected would be truncated by C++: not modeled.
  auto truncated = weak_attribute<int64_t>("Double1", catalog);
  REQUIRE(text(truncated, 0) == "(not translatable)");
  // Strings are not modeled.
  auto string = weak_attribute<int64_t>("String", catalog);
  REQUIRE(text(string, 0) == "(not translatable)");
}

TEST_CASE("The weak attribute node never throws when the catalog cannot answer",
          "[MintermTreeCore][Optimizations][Weak]") {
  // The catalog signals unknown events and attributes with exceptions, which must
  // not escape while an evaluator is being built: the node reports "cannot
  // translate" (an opaque atom) instead.
  CompiledFilter compiled("X[Integer1 > 0]");
  const QueryCatalog& catalog = compiled.query_catalog();
  RecordingBuilder builder;

  auto known = weak_attribute<int64_t>("Integer1", catalog);
  REQUIRE_NOTHROW(known->translate(builder, 99));  // no such event type
  REQUIRE(!known->translate(builder, 99).has_value());

  auto missing = weak_attribute<double>("Double1", catalog);
  REQUIRE_NOTHROW(missing->translate(builder, 1));  // event2 has no Double1
  REQUIRE(!missing->translate(builder, 1).has_value());

  auto unknown_name = weak_attribute<int64_t>("NoSuchAttribute", catalog);
  REQUIRE_NOTHROW(unknown_name->translate(builder, 0));
  REQUIRE(!unknown_name->translate(builder, 0).has_value());
}

// ---------------------------------------------------------------------------
// Attributes stored differently by different event types
// ---------------------------------------------------------------------------

TEST_CASE("The stored type is resolved per event type too",
          "[MintermTreeCore][Optimizations][Weak]") {
  // `Value` is an int in event1 and a double in event2, so a predicate on it works
  // with doubles: the int of event1 is converted, the double of event2 is not.
  CompiledFilter compiled("X[Value > 5]", declare_mixed_types);
  auto predicates = compiled.filter_predicates();
  REQUIRE(predicates.size() == 1);
  REQUIRE(predicates[0]->admissible_event_types == kBothEventTypes);
  REQUIRE(formula_of(*predicates[0], 0) == "(> toDouble(i(0,0)) 5)");
  REQUIRE(formula_of(*predicates[0], 1) == "(> d(1,0) 5)");
}

TEST_CASE("Stored types that are not modeled are opaque, per event type",
          "[MintermTreeCore][Optimizations][Weak]") {
  CompiledFilter compiled("X[Count > 1]", declare_mixed_types);
  const QueryCatalog& catalog = compiled.query_catalog();
  RecordingBuilder builder;

  // Count is an int in both events: modeled for both.
  auto count = weak_attribute<int64_t>("Count", catalog);
  REQUIRE(text_of(builder, count->translate(builder, 0)) == "i(0,4)");
  REQUIRE(text_of(builder, count->translate(builder, 1)) == "i(1,1)");

  // Bool, date and text attributes are not modeled (event1 only).
  for (const char* name : {"Flag", "When", "Text"}) {
    INFO("attribute: " << name);
    auto attribute = weak_attribute<int64_t>(name, catalog);
    REQUIRE(!attribute->translate(builder, 0).has_value());
  }
}

// ---------------------------------------------------------------------------
// A weakly typed OR
// ---------------------------------------------------------------------------

TEST_CASE("A weakly typed OR admits every event type, its children are gated",
          "[MintermTreeCore][Optimizations][Weak]") {
  // The CEQL visitor for aliases builds `X[a OR b]` with the constructor that
  // admits every event type. That is harmless because an Or evaluates its children
  // through operator(): each child only counts for the event types it names. Here
  // Integer1 exists in both event types but Double1 only in event1, so for
  // event2 the second child is gated off and reads as `false`.
  CompiledFilter compiled("X[Integer1 > 5 OR Double1 < 2.5]");
  auto predicates = compiled.filter_predicates();
  REQUIRE(predicates.size() == 1);
  REQUIRE(predicates[0]->admits_any_event_type);
  REQUIRE(formula_of(*predicates[0], 0) == "(or (> i(0,1) 5) (< d(0,3) 2.5))");
  REQUIRE(formula_of(*predicates[0], 1) == "(or (> i(1,0) 5) false)");
}

}  // namespace CORE::Internal::Optimizations::MintermTree::UnitTests::TreeCore
