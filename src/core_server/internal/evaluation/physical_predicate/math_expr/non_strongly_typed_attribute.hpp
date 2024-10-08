#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>

#include "core_server/internal/coordination/query_catalog.hpp"
#include "core_server/internal/evaluation/physical_predicate/compare_with_attribute.hpp"
#include "core_server/internal/stream/ring_tuple_queue/tuple.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
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
  QueryCatalog& query_catalog;

  // If Type == std::string_view, then the underlying string is stored, if not
  // a char is stored.
  typename std::conditional<std::is_same_v<GlobalType, std::string_view>,
                            std::string,
                            char>::type stored_string;

  NonStronglyTypedAttribute(std::string name, QueryCatalog& query_catalog)
      : name(name), query_catalog(query_catalog) {}

  std::unique_ptr<MathExpr<GlobalType>> clone() const override {
    return std::make_unique<NonStronglyTypedAttribute<GlobalType>>(name, query_catalog);
  }

  ~NonStronglyTypedAttribute() override = default;

  GlobalType eval(RingTupleQueue::Tuple& tuple) override {
    // It must be determined at the predicate level whether this eval
    // makes sense for the tuple.
    size_t pos = query_catalog.get_index_attribute(tuple.id(), name);
    const Types::EventInfo& event_info = query_catalog.get_event_info(tuple.id());
    assert(event_info.attribute_names_to_ids.contains(name));
    Types::ValueTypes attribute_type = event_info.attributes_info[pos].value_type;

    switch (attribute_type) {
      case Types::ValueTypes::INT64:
        return eval<int64_t>(tuple, pos);
      case Types::ValueTypes::DOUBLE:
        return eval<double>(tuple, pos);
      case Types::ValueTypes::BOOL:
        return eval<bool>(tuple, pos);
      case Types::ValueTypes::STRING_VIEW:
        return eval<std::string_view>(tuple, pos);
      case Types::ValueTypes::DATE:
        return eval<std::time_t>(tuple, pos);
      default:
        assert(false
               && "A value type was not implemented in NonStronglytypedAttribute eval");
        break;
    }
    return {};  // For warning not to appear.
  }

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
  GlobalType eval(RingTupleQueue::Tuple& tuple, size_t pos) {
    RingTupleQueue::Value<LocalType> val(tuple[pos]);
    if constexpr (std::is_same_v<GlobalType, LocalType>) {
      return val.get();
    } else if constexpr (std::is_same_v<GlobalType, std::string_view>) {
      stored_string = std::to_string(val.get());  // It is not a string already.
      return stored_string;
    } else if constexpr (std::is_same_v<LocalType, std::string_view>) {
      assert(false
             && "Local Type is string and global type is not, this should never happen.");
      return {};
    } else {
      return static_cast<GlobalType>(val.get());
    }
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

  std::string to_string() const override { return name; }
};
}  // namespace CORE::Internal::CEA
