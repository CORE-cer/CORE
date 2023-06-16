#pragma once
#include "core_server/internal/ceql/query/query.hpp"

namespace InternalCORECEQL {

/**
 * This type of polymorphism is called CRTP that allows for interfaces with
 * no overhead created due virtual methods. This allows for documentation
 * and the ability of adding helper functions on methods that implement
 * this inferface
 */
template <class Derived>
class QueryTransformer {
  Query operator()(Query&& query) { static_cast<Derived*>(this)->eval(); }

  Query eval(Query&& query) {
    throw std::logic_error("eval not implemented");
  }
};
}  // namespace InternalCORECEQL
