#pragma once
#include <memory>
#include <string>
#include <vector>

#include "event_type_formula.hpp"
#include "formula.hpp"

namespace CORE::Internal::CEQL {

struct UnlessFormula : public Formula {
  std::unique_ptr<Formula> formula;
  std::unique_ptr<EventTypeFormula> unless;

  // note: candidate constructor (the implicit copy constructor) not viable: requires 1 argument, but 2 were provided

  // note: candidate constructor not viable: no known conversion from
  // 'unique_ptr<CORE::Internal::CEQL::Formula, default_delete<CORE::Internal::CEQL::Formula>>'
  // to 'unique_ptr<CORE::Internal::CEQL::EventTypeFormula, default_delete<CORE::Internal::CEQL::EventTypeFormula>>'
  // for 2nd argument

  UnlessFormula(std::unique_ptr<Formula>&& formula,
                std::unique_ptr<EventTypeFormula>&& unless)
      : formula(std::move(formula)), unless(std::move(unless)) {}

  ~UnlessFormula() override = default;

  // note: in instantiation of function template specialization
  // 'std::make_unique<CORE::Internal::CEQL::UnlessFormula, std::unique_ptr<CORE::Internal::CEQL::Formula>, std::unique_ptr<CORE::Internal::CEQL::Formula>>'

  std::unique_ptr<Formula> clone() const override {
    // error: no matching constructor for initialization of 'CORE::Internal::CEQL::UnlessFormula' unique_ptr<_Tp>
    return std::make_unique<UnlessFormula>(formula->clone(), unless->clone());
  }

  bool operator==(const UnlessFormula& other) const {
    return formula->equals(other.formula.get()) && unless->equals(other.unless.get());
  }

  bool equals(Formula* other) const override {
    if (auto other_formula = dynamic_cast<UnlessFormula*>(other)) {
      return *this == *other_formula;
    } else
      return false;
  }

  std::string to_string() const override {
    return formula->to_string() + " UNLESS " + unless->to_string();
  }

  void accept_visitor(FormulaVisitor& visitor) override { visitor.visit(*this); }
};
}  // namespace CORE::Internal::CEQL