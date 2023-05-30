#pragma once

#include <string>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter.hpp"
#include "formula.hpp"

namespace InternalCORECEQL {

class OrFormula : public Formula {
 private:
  std::unique_ptr<Formula> left;
  std::unique_ptr<Formula> right;

 public:
  OrFormula(std::unique_ptr<Formula>&& left,
            std::unique_ptr<Formula>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~OrFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<OrFormula>(left->clone(), right->clone());
  }

  std::string to_string() const override {
    return left->to_string() + " OR " + right->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace InternalCORECEQL
