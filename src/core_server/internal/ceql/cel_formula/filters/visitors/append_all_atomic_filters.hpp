#pragma once

#include <stdexcept>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter_headers.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "filter_visitor.hpp"

namespace CORE::Internal::CEQL {

class AppendAllAtomicFilters : public FilterVisitor {
 public:
  std::vector<AtomicFilter*>& atomic_filters;

  AppendAllAtomicFilters(std::vector<AtomicFilter*>& atomic_filters)
      : atomic_filters(atomic_filters) {}

  ~AppendAllAtomicFilters() override = default;

  void visit(AtomicFilter& filter) override { atomic_filters.push_back(&filter); }

  // clang-format off
  void visit(AndFilter& filter) override { filter.left->accept_visitor(*this);
                                           filter.right->accept_visitor(*this); }
  void visit(OrFilter& filter)  override { filter.left->accept_visitor(*this);
                                           filter.right->accept_visitor(*this); }

  // clang-format on
};
}  // namespace CORE::Internal::CEQL
