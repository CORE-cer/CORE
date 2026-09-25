#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/evaluation/physical_predicate/formula_builder.hpp"
#include "core_server/internal/evaluation/physical_predicate/math_expr/attribute_formula.hpp"
#include "math_expr.hpp"
#include "shared/datatypes/aliases/event_type_id.hpp"
#include "shared/datatypes/catalog/datatypes.hpp"
#include "shared/datatypes/catalog/event_info.hpp"
#include "shared/datatypes/eventWrapper.hpp"

namespace CORE::Internal::CEA {

template <typename GlobalType>
class NonStronglyTypedAttribute : public MathExpr<GlobalType> {
 public:
  std::string name;
  const QueryCatalog& query_catalog;

  // If Type == std::string_view, then the underlying string is stored, if not
  // a char is stored.
  typename std::conditional<std::is_same_v<GlobalType, std::string_view>,
                            std::string,
                            char>::type stored_string;

  NonStronglyTypedAttribute(std::string name, const QueryCatalog& query_catalog)
      : name(name), query_catalog(query_catalog) {}

  std::unique_ptr<MathExpr<GlobalType>> clone() const override {
    return std::make_unique<NonStronglyTypedAttribute<GlobalType>>(name, query_catalog);
  }

  ~NonStronglyTypedAttribute() override = default;

  GlobalType eval(Types::EventWrapper& event) override {
    // It must be determined at the predicate level whether this eval
    // makes sense for the tuple.
    Types::UniqueEventTypeId unique_event_id = event.get_unique_event_type_id();
    size_t pos = query_catalog.get_index_attribute(unique_event_id, name);
    const Types::EventInfo& event_info = query_catalog.get_event_info(unique_event_id);
    assert(event_info.attribute_names_to_ids.contains(name));
    Types::ValueTypes attribute_type = event_info.attributes_info[pos].value_type;

    switch (attribute_type) {
      case Types::ValueTypes::INT64:
        return eval<int64_t>(event, pos);
      case Types::ValueTypes::DOUBLE:
        return eval<double>(event, pos);
      case Types::ValueTypes::BOOL:
        return eval<bool>(event, pos);
      case Types::ValueTypes::STRING_VIEW:
        return eval<std::string_view>(event, pos);
      case Types::ValueTypes::DATE:
        return eval<std::time_t>(event, pos);
      default:
        assert(false
               && "A value type was not implemented in NonStronglytypedAttribute eval");
        break;
    }
    return {};  // For warning not to appear.
  }

  template <typename LocalType>
  GlobalType eval(Types::EventWrapper& event, size_t pos) {
    const typename ToCoreType<LocalType>::type&
      val = event.get_attribute_at_index<typename ToCoreType<LocalType>::type>(pos);
    if constexpr (std::is_same_v<GlobalType, LocalType>) {
      return val.val;
    } else if constexpr (std::is_same_v<GlobalType, std::string_view>) {
      stored_string = std::to_string(val.val);  // It is not a string already.
      return stored_string;
    } else if constexpr (std::is_same_v<LocalType, std::string_view>) {
      assert(false
             && "Local Type is string and global type is not, this should never happen.");
      return {};
    } else {
      return static_cast<GlobalType>(val.val);
    }
  }

  // The attribute as a formula (see FormulaBuilder), so that predicates on weakly
  // typed variables can be related to each other and to strongly typed ones.
  //
  // eval() finds the attribute anew for every event: it asks the catalog for the
  // position of `name` in the event's type and switches on the type stored there.
  // Nothing needs to be decided per event for the formula, because the minterm
  // tree is built for ONE event type at a time: the same lookup is done once,
  // here, for `event_type`, and the result is described exactly as a strongly
  // typed attribute would be (attribute_formula: the same table, the same Z3
  // variable per (event type, position)). So `T1[price > 5]` and
  // `SELL[price < 3]` talk about the same variable for SELL events even though one
  // resolves the attribute by name and the other by position.
  //
  // Left opaque (nullopt) when the catalog does not know the event type or the
  // event type has no attribute called `name` (the predicate does not admit such
  // events anyway), and for the stored types that are not modeled: bool, strings,
  // dates, the primary time. This is decided per event type: the same predicate
  // may be modeled for one event type and opaque for another. Nothing here throws:
  // the catalog lookups signal failure with exceptions, which must not escape
  // while an evaluator is being built.
  std::optional<FormulaBuilder::Handle>
  translate(FormulaBuilder& builder, Types::UniqueEventTypeId event_type) const override {
    const Types::EventInfo* event_info = nullptr;
    try {
      event_info = &query_catalog.get_event_info(event_type);
    } catch (const std::runtime_error&) {
      return std::nullopt;
    }
    auto attribute = event_info->attribute_names_to_ids.find(name);
    if (attribute == event_info->attribute_names_to_ids.end()) return std::nullopt;
    size_t pos = attribute->second;

    switch (event_info->attributes_info[pos].value_type) {
      case Types::ValueTypes::INT64:
        return attribute_formula<GlobalType, int64_t>(builder, event_type, pos);
      case Types::ValueTypes::DOUBLE:
        return attribute_formula<GlobalType, double>(builder, event_type, pos);
      default:
        return std::nullopt;
    }
  }

  std::string to_string() const override { return name; }
};
}  // namespace CORE::Internal::CEA
