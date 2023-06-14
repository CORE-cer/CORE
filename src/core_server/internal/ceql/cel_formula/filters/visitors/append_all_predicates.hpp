#pragma once

#include <stdexcept>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter_headers.hpp"
#include "core_server/internal/ceql/cel_formula/predicate/predicate.hpp"
#include "filter_visitor.hpp"

namespace InternalCORECEQL {

class AppendAllPredicates : public FilterVisitor {
 public:
  std::vector<Predicate*>& predicates;

  AppendAllPredicates(std::vector<Predicate*>& predicates)
      : predicates(predicates) {}

  ~AppendAllPredicates() override = default;

  void visit(AtomicFilter& filter) override {
    predicates.push_back(filter.predicate.get());
  }

  // clang-format off
  void visit(AndFilter& filter) override { filter.left->accept_visitor(*this);
                                           filter.right->accept_visitor(*this); }
  void visit(OrFilter& filter)  override { filter.left->accept_visitor(*this);
                                           filter.right->accept_visitor(*this); }

  // clang-format on
};
}  // namespace InternalCORECEQL
