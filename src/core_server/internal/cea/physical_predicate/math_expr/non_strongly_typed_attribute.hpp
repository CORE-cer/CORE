#pragma once
#include <cassert>
#include <memory>

#include <cassert>

#include "core_server/internal/coordination/catalog.hpp"
#include "core_server/internal/stream/ring_tuple_queue/value.hpp"
#include "math_expr.hpp"

namespace InternalCORECEA {

template <typename Type>
class NonStronglyTypedAttribute : public MathExpr<Type> {
 public:
  std::string name;
  InternalCORE::Catalog& catalog;

  NonStronglyTypedAttribute(std::string name, InternalCORE::Catalog& catalog)
      : name(name), catalog(catalog) {}

  std::unique_ptr<MathExpr<Type>> clone() const override {
    return std::make_unique<NonStronglyTypedAttribute<Type>>(name, catalog);
  }

  ~NonStronglyTypedAttribute() override = default;

  Type eval(RingTupleQueue::Tuple& tuple) override {
    size_t pos;
    EventInfo event_info = catalog.get_event_info(tuple.id());
    // It must be determined at the predicate level whether this eval
    // makes sense for the tuple.
    assert(event_info.attribute_names_to_ids.contains(name));
    pos = event_info.attribute_names_to_ids[name];
    RingTupleQueue::Value<Type> val(tuple[pos]);
    return val.get();
  }

  std::string to_string() const override { return name; }
};
}  // namespace InternalCORECEA
