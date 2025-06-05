#pragma once
#include <stdexcept>
#include <utility>

#include "core_server/internal/ceql/query/query.hpp"

namespace CORE::Internal::CEQL {

template <class Derived>
class QueryTransformer {
  QueryTransformer() = default;
  friend Derived;

 public:
  Query operator()(Query&& query) {
    return static_cast<Derived*>(this)->eval(std::move(query));
  }

  Query eval(Query&& query) { throw std::logic_error("eval not implemented"); }
};
}  // namespace CORE::Internal::CEQL
