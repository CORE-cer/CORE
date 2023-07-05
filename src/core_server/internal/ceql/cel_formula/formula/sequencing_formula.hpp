#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core_server/internal/ceql/cel_formula/filters/filter.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct SequencingFormula : public Formula {
  std::unique_ptr<Formula> left;
  std::unique_ptr<Formula> right;

  SequencingFormula(std::unique_ptr<Formula>&& left,
                    std::unique_ptr<Formula>&& right)
      : left(std::move(left)), right(std::move(right)) {}

  ~SequencingFormula() override = default;

  std::unique_ptr<Formula> clone() const override {
    return std::make_unique<SequencingFormula>(left->clone(),
                                               right->clone());
  }

  bool operator==(const SequencingFormula& other) const {
    return left->equals(other.left.get())
           && right->equals(other.right.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<SequencingFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return "(" + left->to_string() + ";" + right->to_string() + ")";
  }

  void accept_visitor(FormulaVisitor& visitor) override {
    visitor.visit(*this);
  }
};
}  // namespace CORE::Internal::CEQL
