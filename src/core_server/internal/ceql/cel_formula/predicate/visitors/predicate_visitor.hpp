#pragma once

#include <stdexcept>

namespace CORE::Internal::CEQL {

class AndPredicate;
class InPredicate;
class InequalityPredicate;
class LikePredicate;
class NotPredicate;
class OrPredicate;
class ConstantBooleanPredicate;

class PredicateVisitor {
 public:
  virtual ~PredicateVisitor() = default;

  // clang-format off
  virtual void visit(AndPredicate&)                     {throw std::logic_error("visit AndPredicate not implemented.");}
  virtual void visit(InPredicate&)                      {throw std::logic_error("visit InPredicate not implemented.");}
  virtual void visit(InequalityPredicate&)              {throw std::logic_error("visit InequalityPredicate not implemented.");}
  virtual void visit(LikePredicate&)                    {throw std::logic_error("visit LikePredicate not implemented.");}
  virtual void visit(NotPredicate&)                     {throw std::logic_error("visit NotPredicate not implemented.");}
  virtual void visit(OrPredicate&)                      {throw std::logic_error("visit OrPredicate not implemented.");}
  virtual void visit(ConstantBooleanPredicate&)         {throw std::logic_error("visit ConstantBooleanPredicate not implemented.");}

  // clang-format on
};
}  // namespace CORE::Internal::CEQL
