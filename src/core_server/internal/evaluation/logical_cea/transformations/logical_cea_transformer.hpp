#pragma once
#include <stdexcept>
#include <utility>
#include "core_server/internal/evaluation/logical_cea/logical_cea.hpp"

namespace CORE::Internal::CEA {

/**
 * This type of polymorphism is called CRTP that allows for interfaces with
 * no overhead created due virtual methods. This allows for documentation
 * and the ability of adding helper functions on methods that implement
 * this inferface
 */
template <class Derived>
class LogicalCEATransformer {
 public:
  LogicalCEA operator()(LogicalCEA&& cea) {
    return static_cast<Derived*>(this)->eval(std::move(cea));
  }

  LogicalCEA operator()(LogicalCEA& left, LogicalCEA& right) {
    return static_cast<Derived*>(this)->eval(left, right);
  }

  LogicalCEA eval(LogicalCEA&& query) {
    throw std::logic_error("eval(query) not implemented");
  }

  LogicalCEA eval(LogicalCEA&& left, LogicalCEA&& right) {
    throw std::logic_error("eval(left, right) not implemented");
  }
};

}  // namespace CORE::Internal::CEA
