#pragma once

#include <memory>

#include "visitors/formula_visitor.hpp"

namespace InternalCORECEQL {

class Formula {
  public:
    virtual ~Formula() = default;
    virtual std::unique_ptr<Formula> clone() const = 0;
    virtual bool equals(Formula* other) const = 0;
    virtual std::string to_string() const = 0;
    virtual void accept_visitor(FormulaVisitor&) = 0;
};
}  // namespace InternalCORECEQL
